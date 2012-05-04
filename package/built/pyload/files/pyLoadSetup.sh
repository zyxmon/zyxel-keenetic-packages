#!/bin/sh

MOUNT="/media/DISK_A1/system"

export PATH=$MOUNT/bin:$MOUNT/sbin:$MOUNT/usr/bin:$MOUNT/usr/sbin:/sbin:/usr/sbin:/bin:/usr/bin
export LD_LIBRARY_PATH=$MOUNT/lib:$MOUNT/usr/lib:/lib:/usr/lib


down() {
	if [ -d $MOUNT/usr/share/pyload ]; then
		echo "pyLoad is already installed"
		exit 1
	fi
	cd $MOUNT/usr/share
	wget http://get.pyload.org/get/src/ -O $MOUNT/usr/share/pyload-src.zip
	unzip $MOUNT/usr/share/pyload-src.zip
	rm -f $MOUNT/usr/share/pyload-src.zip
	echo "/media/DISK_A1/pyload" > $MOUNT/usr/share/pyload/module/config/configdir
	
}

remove() {
	if [ ! -d $MOUNT/usr/share/pyload ]; then
		echo "pyLoad is not installed"
		exit 1
	fi
	rm -fr /$MOUNT/usr/share/pyload
}

setup() {
	if [ ! -d $MOUNT/usr/share/pyload ]; then
		echo "pyLoad is not installed"
		exit 1
	fi
	python $MOUNT/usr/share/pyload/pyLoadCore.py -s
}

users() {
	if [ ! -d $MOUNT/usr/share/pyload ]; then
		echo "pyLoad is not installed"
		exit 1
	fi
	python $MOUNT/usr/share/pyload/pyLoadCore.py -u
}

case "$1" in
	down)
		down
		;;
	remove)
		remove
		;;
	setup)
		setup
		;;
	users)
		users
		;;
	*)
		echo "Usage: $0 {down|remove|setup|users}"
		;;
esac