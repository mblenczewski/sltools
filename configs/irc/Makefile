BIN = irc

CFLAGS = -std=c99 -Os $(shell pkg-config --cflags ncursesw) -D_POSIX_C_SOURCE=201112 -D_GNU_SOURCE -D_XOPEN_CURSES -D_XOPEN_SOURCE_EXTENDED=1 -D_BSD_SOURCE
LDFLAGS = $(shell pkg-config --libs ncursesw) -lssl -lcrypto

all: ${BIN}

clean:
	rm -f ${BIN} *.o

.PHONY: all clean
