#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#define ARRLEN(arr) (sizeof (arr) / sizeof (arr)[0])

#define NSECS 1000000000UL

#define BUFSZ 8192
static char statbuf[BUFSZ];

#define BLOCKSEP " | "

struct block { u64 (*fn)(char *buf, u64 len); };

u64 cpuinfo(char *buf, u64 len);
u64 meminfo(char *buf, u64 len);
u64 batinfo(char *buf, u64 len);
u64 datetime(char *buf, u64 len);

static struct block blocks[] = {
	{ .fn = cpuinfo, },
	{ .fn = meminfo, },
//	{ .fn = batinfo, },
	{ .fn = datetime, },
};

static inline void
difftimespec(struct timespec const *restrict lhs, struct timespec const *restrict rhs,
	     struct timespec *restrict out) {
	out->tv_sec = lhs->tv_sec - rhs->tv_sec - (lhs->tv_nsec < rhs->tv_nsec);
	out->tv_nsec = lhs->tv_nsec - rhs->tv_nsec + (lhs->tv_nsec < rhs->tv_nsec) * NSECS;
}

static char *somebar_path = NULL;

void
read_env_vars(void) {
	somebar_path = strcat(getenv("XDG_RUNTIME_DIR"), "/somebar-0");
}

void
exec_blocks(void) {
	u64 written = 0, limit = BUFSZ - 1;
	for (u64 i = 0; i < ARRLEN(blocks) && written < limit; i++) {
		written += blocks[i].fn(statbuf + written, limit - written);
	}

	statbuf[written] = '\0';
}

void
output_status(void) {
	static int somebarfd = -1;

	if (somebarfd < 0) {
		if ((somebarfd = open(somebar_path, O_WRONLY|O_CLOEXEC)) < 0 && errno == ENOENT) {
			sleep(1); // TODO: wait for somebar path to appear
			somebarfd = open(somebar_path, O_WRONLY|O_CLOEXEC);
		}

		if (somebarfd < 0) {
			perror("open");
			exit(EXIT_FAILURE);
		}
	}

	dprintf(somebarfd, "status %s\n", statbuf);
}

int
main(void) {
	read_env_vars();

	struct timespec start, end, diff, wait, interval = { .tv_sec = 1, };
	while (true) {
		if (clock_gettime(CLOCK_MONOTONIC, &start) < 0) {
			perror("clock_gettime");
			exit(EXIT_FAILURE);
		}

		exec_blocks();
		output_status();

		if (clock_gettime(CLOCK_MONOTONIC, &end) < 0) {
			perror("clock_gettime");
			exit(EXIT_FAILURE);
		}

		difftimespec(&end, &start, &diff);
		difftimespec(&interval, &diff, &wait);

		if (wait.tv_sec >= 0 && nanosleep(&wait, NULL) < 0 && errno != EINTR) {
			perror("nanosleep");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}

/* ===========================================================================
 * status bar blocks
 * ===========================================================================
 */

static int
pscanf(const char *path, const char *fmt, ...) {
	FILE *fp;
	va_list ap;
	int n;

	if (!(fp = fopen(path, "r"))) {
		fprintf(stderr, "fopen '%s':", path);
		return -1;
	}
	va_start(ap, fmt);
	n = vfscanf(fp, fmt, ap);
	va_end(ap);
	fclose(fp);

	return (n == EOF) ? -1 : n;
}

u64
cpuinfo(char *buf, u64 len) {
	static long double a[7];
	long double b[7], sum;
	s32 usage;

	memcpy(b, a, sizeof(b));

	/* cpu user nice system idle iowait irq softirq */
	if (pscanf("/proc/stat", "%*s %LF %LF %LF %LF %LF %LF %LF",
	           &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6]) != 7) {
		perror("vfscanf");
		exit(EXIT_FAILURE);
	}

	sum = (b[0] + b[1] + b[2] + b[3] + b[4] + b[5] + b[6]) -
	      (a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6]);

	if (sum == 0) {
		usage = 0;
	} else {
		usage = (s32)(100 * 
			     ((b[0] + b[1] + b[2] + b[5] + b[6]) -
			      (a[0] + a[1] + a[2] + a[5] + a[6])) / sum);
	}

	u64 written;
	if (!(written = snprintf(buf, len, "CPU: %" PRIi32 "%%" BLOCKSEP, usage))) {
		perror("snprintf");
		exit(EXIT_FAILURE);
	}

	return written;
}

u64
meminfo(char *buf, u64 len) {
	uintmax_t total, free, buffers, cached;
	int percent;

	if (pscanf("/proc/meminfo",
	           "MemTotal: %ju kB\n"
	           "MemFree: %ju kB\n"
	           "MemAvailable: %ju kB\n"
	           "Buffers: %ju kB\n"
	           "Cached: %ju kB\n",
	           &total, &free, &buffers, &buffers, &cached) != 5) {
		perror("vfscanf");
		exit(EXIT_FAILURE);
	}

	if (total == 0) {
		percent = 0;
	} else {
		percent = 100 * ((total - free) - (buffers + cached)) / total;
	}

	u64 written;
	if (!(written = snprintf(buf, len, "MEM: %" PRIi32 "%%" BLOCKSEP, percent))) {
		perror("snprintf");
		exit(EXIT_FAILURE);
	}

	return written;
}

u64
batinfo(char *buf, u64 len) {
	s32 capacity;
	char status[13]; // "Charging" | "Discharging" | "Full" | "Not Charging"

	if (pscanf("/sys/class/power_supply/BAT0/capacity", "%" SCNi32 "", &capacity) != 1) {
		perror("vfscanf");
		exit(EXIT_FAILURE);
	}

	if (pscanf("/sys/class/power_supply/BAT0/status", "%12[a-zA-Z ]", &status) != 1) {
		perror("vfscanf");
		exit(EXIT_FAILURE);
	}

	u64 written;
	if (!(written = snprintf(buf, len, "BAT: %" PRIi32 "%% %12s" BLOCKSEP, capacity, status))) {
		perror("snprintf");
		exit(EXIT_FAILURE);
	}

	return written;
}

u64
datetime(char *buf, u64 len) {
	char const *timefmt = "%F %T";
	time_t t = time(NULL);

	u64 written;
	if (!(written = strftime(buf, len, timefmt, localtime(&t)))) {
		perror("strftime");
		exit(EXIT_FAILURE);
	}

	return written;
}
