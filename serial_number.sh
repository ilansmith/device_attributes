#!/bin/bash

if [ $# -ne 1 ]
then
	echo "usage: $0 <ip-addr>"
	exit -1
fi

IP=$1

DEVICE="`ifconfig | grep -B1 \"inet $1\"`"
IPX="`echo $DEVICE | awk -F' ' '{ print $6 }'`"
if [ "$IP" != "$IPX" ]
then
	echo "no device found with ip: $1"
	exit -2
fi

DEVICE="`echo $DEVICE | awk -F':' '{ print $1 }'`"
PCI_ADDR="`mst status -v | grep $DEVICE | awk -F' ' '{ print $3 }'`"
#echo $PCI_ADDR

SN="`lspci -vv -s $PCI_ADDR | grep \"Serial number\" | awk -F' ' '{ print $4 }'`"
echo $SN
