BIN = irc

CURSESINC = `pkg-config --cflags ncursesw`
CURSESLIB = `pkg-config --libs ncursesw`

INCS = $(CURSESINC)
LIBS = $(CURSESLIB) -lssl -lcrypto

CFLAGS = -std=c99 -Os -D_POSIX_C_SOURCE=201112 -D_GNU_SOURCE -D_XOPEN_CURSES -D_XOPEN_SOURCE_EXTENDED=1 -D_DEFAULT_SOURCE -D_BSD_SOURCE $(INCS)
LDFLAGS = $(LIBS)

PREFIX=/usr/local

all: ${BIN}

install: ${BIN}
	mkdir -p ${PREFIX}/bin
	cp -f ${BIN} ${PREFIX}/bin
	chmod 755 ${PREFIX}/bin/${BIN}

uninstall:
	rm -f ${PREFIX}/bin/${BIN}

clean:
	rm -f ${BIN} *.o

.PHONY: all clean
