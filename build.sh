#!/bin/sh

. "$(dirname $0)/common.sh"

for tool in $TOOLS; do
	SOURCE "$tool"
	CONFIG "$tool"
	PATCH  "$tool"
	cd "$tool"
	make clean all
	cd ..
done
