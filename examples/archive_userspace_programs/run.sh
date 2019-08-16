#! /bin/bash

DIR=$(cd `dirname $BASH_SOURCE[0]` && pwd -P)

echo "This must be run on the BeagleBone Black itself"
echo "Tested on kernel 4.9.36-ti-r46. Firmware Code taken from the BeagleScope project"
echo "https://github.com/ZeekHuge/BeagleScope/tree/port_to_4.4.12-ti-r31%2B/examples/firmware_exmples"



echo "-Building Firmware"
	cd "${DIR}/firmware_examples/$1/"
	make clean
	make

echo "-Compiling bindings"
	cd ../../
	g++ -o $1.o ../cpp-bindings/pruss.h ../cpp-bindings/pruss.cpp $1.cpp
echo "Compiled the bindings"

echo "-Setting pinmux"
	config-pin P8.46 pruout
	config-pin P8.45 pruin
echo "Pin 8.45 configured as PRU input for pru_pin_state_reader"
echo "Pin 8.46 configured as PRU output for blinky"
