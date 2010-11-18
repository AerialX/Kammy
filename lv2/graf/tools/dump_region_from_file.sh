#!/bin/sh

if [ $# -ne 3 ]; then
	echo "usage: $0 <file name> <offset> <size>" >&2
	exit 1
fi

filename=$1
offset=$(($2))
size=$(($3))

dd if="$filename" bs=1 skip=$offset count=$size
