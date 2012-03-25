#!/bin/sh

#pptp
iptables -I INPUT -p tcp --dport 1723 -j ACCEPT
iptables -t nat -I PREROUTING -p tcp --dport 1723 -j ACCEPT

#pptp internet
#iptables -I FORWARD -i ppp+ -j ACCEPT
#iptables -I FORWARD -o ppp+ -j ACCEPT
#iptables -I OUTPUT -o ppp+ -j ACCEPT
#iptables -I INPUT -i ppp+ -j ACCEPT

#pptp client-to-client
iptables -I FORWARD -i ppp+ -o ppp+ -j ACCEPT
