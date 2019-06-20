#!/bin/bash
export PRUN=1
export TARGET=hello
echo PRUN=$PRUN
echo TARGET=$TARGET

echo none > /sys/class/leds/beaglebone\:green\:usr3/trigger
