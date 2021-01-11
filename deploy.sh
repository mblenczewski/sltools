#!/bin/sh

echo "Please run this script as the root user!"
read -p "Press enter to continue..." _

CONFIGS=configs
PATCHES=patches

# $1 : program name
# $2 : target dir
APPLY_CONF () {
	for f in $(find $CONFIGS/$1/ -name '*.*'); do
		cp $f $2
	done
}

# $1 : program name
# $2 : target dir
APPLY_PATCHES () {
	for f in $(find $PATCHES/$1/ -name '*.diff'); do
		patch -d$2 <$f
	done
}

rm -rf st/ dwm/ slstatus/ slock/ irc/

## st
git clone $(cat st.git)

APPLY_CONF st st
APPLY_PATCHES st st

cd st

make clean install

cd ..


## dwm
git clone $(cat dwm.git)

APPLY_CONF dwm dwm
APPLY_PATCHES dwm dwm

cd dwm

make clean install

cd ..


## slstatus
git clone $(cat slstatus.git)

APPLY_CONF slstatus slstatus
APPLY_PATCHES slstatus slstatus

cd slstatus

make clean install

cd ..


## slock
git clone $(cat slock.git)

APPLY_CONF slock slock
APPLY_PATCHES slock slock

cd slock

make clean install

cd ..


## irc
git clone $(cat irc.git)

APPLY_CONF irc irc
APPLY_PATCHES irc irc

cd irc

make clean && make
cp irc /usr/local/bin

cd ..

