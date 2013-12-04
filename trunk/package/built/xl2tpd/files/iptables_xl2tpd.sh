#!/bin/sh

#xl2tpd
iptables -I INPUT -p udp --dport 1701 -j ACCEPT
