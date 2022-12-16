#!/bin/sh

. "$(dirname $0)/common.sh"

PREFIX="${PREFIX:-/usr/local}"

for tool in $TOOLS; do
	make -C "$tool" install PREFIX="$PREFIX"
done
