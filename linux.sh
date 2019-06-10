#!/bin/bash

echo $#
exit

IP=$1

DEVICE="`ifconfig | grep -B1 \"inet $1\" | grep -v inet | awk -F':' '{ print $1}'`"
PCI_ADDR="`mst status -v | grep $DEVICE | awk -F' ' '{ print $3}'`"

echo $PCI_ADDR
