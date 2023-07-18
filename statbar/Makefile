.PHONY: all build clean install uninstall

CFLAGS	:= -std=c17 -Wall -Wextra -Wpedantic -Og -ggdb
CPPFLAGS:= -D_XOPEN_SOURCE=700
LDFLAGS	:=

CC	?= cc

PREFIX	?= /usr/local

all: build

build: statbar-wl

clean:
	rm -rf statbar-wl $(OBJ)

install: build
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 0755 statbar-wl $(DESTDIR)$(PREFIX)/bin/statbar-wl

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/statbar-wl

statbar-wl: statbar-wl.c
	$(CC) -o $@ $^ $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
