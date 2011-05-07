#!/bin/sh

# This script creates proper resolv.conf and starts dnrd daemon as DNS proxy

RESOLV=/etc/resolv.conf
UDHCP_RESOLV=/var/udhcpc/resolv.conf
PPP_RESOLV=/etc/ppp/resolv.conf
PIDFILE=/var/run/dnrd.pid
DNRD_MASTER=/etc/dnrd/master
EZTUNE_FILE=/var/eztune

if [ "$1" != "start" ]; then
	dnrd -u 0 -k 2> /dev/null
	if [ -f $PIDFILE ]; then
		PID=`cat $PIDFILE`
		kill -9 $PID 2> /dev/null
		rm -f $PIDFILE 2> /dev/null
	fi
	killall -9 dnrd 2> /dev/null
fi

[ "$1" = "stop" ] && \
	exit 0

eval `flash WAN_DNS_MODE WAN_IP_ADDRESS_MODE WAN_DNS1 WAN_DNS2 WAN_DNS3 OP_MODE TRAP_GATE_IP`

echo -n > $RESOLV
if [ "$WAN_DNS_MODE" = 'Enabled' -a "$WAN_IP_ADDRESS_MODE" = 'Auto' ]; then
	if [ -f $UDHCP_RESOLV ]; then
		cat $UDHCP_RESOLV >> $RESOLV
	fi
	if [ -f $PPP_RESOLV ]; then
		cat $PPP_RESOLV >> $RESOLV
	fi
else
	if [ -f $PPP_RESOLV ]; then
		cat $PPP_RESOLV >> $RESOLV
	fi
	if [ "$WAN_DNS1" != '0.0.0.0' ]; then 
		echo "nameserver $WAN_DNS1" >> $RESOLV 
	fi 
	if [ "$WAN_DNS2" != '0.0.0.0' ]; then 
		echo "nameserver $WAN_DNS2" >> $RESOLV 
	fi 
	if [ "$WAN_DNS3" != '0.0.0.0' ]; then 
		echo "nameserver $WAN_DNS3" >> $RESOLV 
	fi 
fi

DNS=""
for serv in `cat $RESOLV | grep nameserver | head -n 10 | cut -d" " -f 2`; do 
	DNS="$DNS -s $serv"
done

EXT_OPT="-r 0"
#if [ "$OP_MODE" = '3G Router' -o "$OP_MODE" = 'WiMAX Router' ]; then
#	EXT_OPT="-r 0"
#fi

if [ -f $EZTUNE_FILE ]; then
	echo "domain *" > $DNRD_MASTER
	echo ${TRAP_GATE_IP}1 >> $DNRD_MASTER
	EXT_OPT="$EXT_OPT -T 1"
else
	rm -f $DNRD_MASTER
fi

if [ "$DNS" != "" ] || [ -f $EZTUNE_FILE ]; then
	dnrd -l -d 3 -u 0 -b $EXT_OPT $DNS
fi

