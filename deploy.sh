#!/bin/sh

PREFIX="${PREFIX:-$HOME/.local}"
CONFIGS=$(pwd)/configs
PATCHES=$(pwd)/patches

# $1 : program name
APPLY_CONF () {
	[ -d $CONFIGS/$1 ] && for f in $(find $CONFIGS/$1/ -type f); do
		cp $f .
	done
}

# $1 : program name
APPLY_PATCHES () {
	[ -d $PATCHES/$1 ] && for f in $(find $PATCHES/$1/ -name '*.diff'); do
		patch -N <$f
	done
}

[ -d dmenu ] || git clone $(cat dmenu.git)
cd dmenu
git pull
APPLY_CONF dmenu
APPLY_PATCHES dmenu
make clean install PREFIX="$PREFIX"
cd ..

[ -d dwm ] || git clone $(cat dwm.git)
cd dwm
git pull
APPLY_CONF dwm
APPLY_PATCHES dwm
make clean install PREFIX="$PREFIX"
cd ..


[ -d irc ] || git clone $(cat irc.git)
cd irc
git pull
APPLY_CONF irc
APPLY_PATCHES irc
make clean && make
cp irc "$PREFIX/bin"
cd ..


[ -d slock ] || git clone $(cat slock.git)
cd slock
git pull
APPLY_CONF slock
APPLY_PATCHES slock
make clean install PREFIX="$PREFIX"
cd ..


[ -d slstatus ] || git clone $(cat slstatus.git)
cd slstatus
git pull
APPLY_CONF slstatus
APPLY_PATCHES slstatus PREFIX="$PREFIX"
make clean install
cd ..


[ -d st ] || git clone $(cat st.git)
cd st
git pull
APPLY_CONF st
APPLY_PATCHES st
make clean install PREFIX="$PREFIX"
cd ..


[ -d surf ] || git clone $(cat surf.git)
cd surf
git pull
APPLY_CONF surf
APPLY_PATCHES surf
make clean install PREFIX="$PREFIX"
cd ..

