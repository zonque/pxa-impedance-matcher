#!/bin/sh

OUT="$1"
shift
DTBS="$*"

cat $DTBS >>$OUT
dd if=/dev/zero of=$OUT oflag=append conv=notrunc bs=1 count=8 #sentinel
