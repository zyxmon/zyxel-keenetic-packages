#!/bin/sh

MOUNT="/storage/system"
FWD="$MOUNT/etc/firewall.d"

export PATH=$MOUNT/bin:$MOUNT/sbin:$MOUNT/usr/bin:$MOUNT/usr/sbin:/sbin:/usr/sbin:/bin:/usr/bin
export LD_LIBRARY_PATH=$MOUNT/lib:$MOUNT/usr/lib:/lib:/usr/lib

start() {
	for prog in `ls $FWD`; do
	    $FWD/$prog
	done
}

start
