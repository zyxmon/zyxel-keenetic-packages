#!/bin/sh

MOUNT="/storage/system"
INITD="$MOUNT/etc/init.d"

export PATH=$MOUNT/bin:$MOUNT/sbin:$MOUNT/usr/bin:$MOUNT/usr/sbin:/sbin:/usr/sbin:/bin:/usr/bin
export LD_LIBRARY_PATH=$MOUNT/lib:$MOUNT/usr/lib:/lib:/usr/lib


start() {
	for prog in `ls $INITD/S??*`; do
	    $prog start $2
	done
}

stop() {
	for prog in `ls -r $INITD/S??*`; do
	    $prog stop $2
	done
}

if [ ! -f /var/tmp/.profile ]; then
echo "export PATH=/storage/system/bin:/storage/system/sbin:/storage/system/usr/bin:/storage/system/usr/sbin:/bin:/sbin:/usr/bin:/usr/sbin
export LD_LIBRARY_PATH=/storage/system/lib:/storage/system/usr/lib:/lib:/usr/lib" > /var/tmp/.profile
fi

case "$1" in
	start)
	    start
	    ;;
	stop)
	    stop
	    ;;
	restart)
	    stop
	    start
	    ;;
	link_up)
	    ;;
	ppp_up)
	    ;;
	link_down)
	    ;;
	ppp_down)
	    ;;
	*)
	    echo "Usage: $0 {start|stop|restart|link_up|link_down|ppp_up|ppp_down}"
	    ;;
esac
