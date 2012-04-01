#!/bin/sh
if [ $4 ]; then  
	echo $1:`/media/DISK_A1/system/usr/bin/mycrypt $$ $2` >> /media/DISK_A1/system/etc/3proxy/passwd
	echo countin \"`wc -l /media/DISK_A1/system/etc/3proxy/counters|awk '{print $1}'`/$1\" D $3 $1 >> /media/DISK_A1/system/etc/3proxy/counters
	echo bandlimin $4 $1 >> /media/DISK_A1/system/etc/3proxy/bandlimiters
else
	echo usage: $0 username password day_limit bandwidth
	echo "	"day_limit - traffic limit in MB per day
	echo "	"bandwidth - bandwith in bits per second 1048576 = 1Mbps
fi
