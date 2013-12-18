#!/bin/sh

OUT="$1"
shift
DTBS="$*"

while [ -n "$*" ]
do
	SIZE=`stat $1 -c %s`
	MOD=`expr $SIZE % 4`
	if [ $MOD -ne 0 ]; then
		PADDING=`expr 4 - $MOD`
		dd if=/dev/zero count=$PADDING bs=1 >> $1 2> /dev/null
	fi
	shift
done

cat $DTBS >>$OUT
dd if=/dev/zero of=$OUT oflag=append conv=notrunc bs=1 count=8 #sentinel
