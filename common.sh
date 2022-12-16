#!/bin/sh

set -e

SOURCES="$(pwd)/sources"
CONFIGS="$(pwd)/configs"
PATCHES="$(pwd)/patches"

SOURCE() {
	if [ ! -d "$1" ]; then
		[ -f "$SOURCES/$1.git" ] && git clone $(cat "$SOURCES/$1.git") "$1"
	else
		__OLD_DIR="$(pwd)"; cd "$1"
		git reset --hard
		git clean -qf
		cd "$__OLD_DIR"
	fi
}

CONFIG() {
	[ -d "$CONFIGS/$1" ] && for f in $(find "$CONFIGS/$1" -type f); do
		cp $f "$1"
	done
}

PATCH() {
	[ -d "$PATCHES/$1" ] && for f in $(find "$PATCHES/$1" -name '*.diff'); do
		__OLD_DIR="$(pwd)"; cd "$1"
		echo "Applying $f..."
		patch -N <$f
		cd "$__OLD_DIR"
	done
}

TOOLS="dwm slstatus slock st dmenu surf irc"
