#!/bin/bash

# Install script for prussd daemon service
# To be run on the BeagleBone, from the cloned PRUSS-Bindings
# git repository

DIR=$(cd `dirname $BASH_SOURCE[0]` && pwd -P)

pru_examples() {
	
	#paths needed to compile PRU firmware examples
	if [ ! -d /usr/share/ti/cgt-pru/bin ] ; 
	then
		ln -s /usr/bin/ /usr/share/ti/cgt-pru/bin
	fi
	export PRU_CGT=/usr/share/ti/cgt-pru

	#run compilation scripts
	bash "${DIR}/examples/run.sh" blinky
	bash "${DIR}/examples/run.sh" rpmsg_echo
	bash "${DIR}/examples/run.sh" pru_pin_state_reader
	bash "${DIR}/examples/run.sh" pru1_to_pru0_to_arm
	bash "${DIR}/examples/run.sh" multi_threaded

}

systemd_service() {

	#copy the systemd unit configuration file
	cp -v "${DIR}/prussd/prussd.service" "/lib/systemd/system/prussd.service"

	#copy the config file
	cp -v "${DIR}/prussd/prussd.conf" "/etc/default/prussd.conf"

	#edit the unit file to replace DIR by the actual path
	sed -i -e "s:DIR:${DIR}:" "/lib/systemd/system/prussd.service"
	
	#edit the config file to include paths to firmware_examples
	sed -i -e "s:DIR:${DIR}:" "/etc/default/prussd.conf"
	
	#mark the daemon script as executable
	chmod a+x "${DIR}/prussd/prussd.py"

	#enable the service
	systemctl enable prussd.service

	#reload the systemctl daemons
	systemctl daemon-reload
	
	#start the prussd daemon
	systemctl start prussd.service 

}

kernel_driver() {
	
	#Install the kernel driver present in drivers/ directory to /lib/modules/$(uname -r) in order to install it
	sudo cp ./drivers/pruss_api.ko /lib/modules/$(uname -r)/
	echo "Copying pruss_api.ko to /lib/modules/$(uname -r)"
	sudo depmod -ae
	echo "Installed the pruss_api driver"

}

if [ $# -eq 0 ]; then
	pru_examples
	systemd_service
	kernel_driver
fi

if [ "$1" == "--examples" ] ; then
	pru_examples
fi

if [ "$1" == "--service" ] ; then
	systemd_service
fi

if [ "$1" == "--driver" ] ; then
	kernel_driver
fi
