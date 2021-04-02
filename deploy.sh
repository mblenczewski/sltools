#!/bin/sh

echo "Please run this script as the root user!"
read -p "Press enter to continue..." _

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
		patch <$f
	done
}

rm -rf dmenu/ dwm/ irc/ slock/ slstatus/ st/ surf/


git clone $(cat dmenu.git)
cd dmenu
APPLY_CONF dmenu
APPLY_PATCHES dmenu
make clean install
cd ..


git clone $(cat dwm.git)
cd dwm
APPLY_CONF dwm
APPLY_PATCHES dwm
make clean install
cd ..


git clone $(cat irc.git)
cd irc
APPLY_CONF irc
APPLY_PATCHES irc
make clean && make
cp irc /usr/local/bin
cd ..


git clone $(cat slock.git)
cd slock
APPLY_CONF slock
APPLY_PATCHES slock
make clean install
cd ..


git clone $(cat slstatus.git)
cd slstatus
APPLY_CONF slstatus
APPLY_PATCHES slstatus
make clean install
cd ..


git clone $(cat st.git)
cd st
APPLY_CONF st
APPLY_PATCHES st
make clean install
cd ..


git clone $(cat surf.git)
cd surf
APPLY_CONF surf
APPLY_PATCHES surf
make clean install
cd ..

