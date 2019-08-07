# PRUSS Bindings
Language Bindings to interact with the PRUs on the Beaglebone.

## Overview
The `cpp-bindings` interact with the Python Daemon Service through UNIX Domain Sockets.
cpp-bindings contains functions that the userspace program will use; the cpp-bindings would then pass the appropriate request to the daemon through the socket file (path: /tmp/prussd.sock). The daemon performs the required task with the PRU and sends back the return value. The advantage of having a daemon is that userspace apps don't need to deal with root permissions, although restricting access to the socket is necessary.
The bindings for other scripting languages can then be built upon the cpp-bindings using SWIG, which generates wrapper functions for C/C++ programs. Bindings have also been provided for 'C' language in case the user wants to work with C instead of using OOP.

## Description
* `prussd` : The Directory which contains the files related to the daemon which will run as a system service and serve PRU requests.
* `cpp-bindings` : The Directory which contains the C++ bindings using the prussd daemon service (therefore using the PRUs without root access)
* `c-bindings` : The Directory which contains the C bindings using the prussd daemon service.
* `examples` : The directory which contains the example codes using the bindings. Contains firmware examples as well.
* `swig` : The Directory which contains the swig config and interface files
* `Documentation` : Contains the API Documentation/Specification
* `install.sh` : The install script for the project
* `drivers` : A dedicated rpmsg driver for the bindings called `pruss_api`
* `python` : The Directory which contains the PyPRUSS project (Python Bindings for the PRUs)[Discontinued]

## Installation Guide

* `config-pin`: requires changes in uEnv.txt - HDMI should be disabled. Otherwise this error is encountered:<br>
```
P9_31 pinmux file not found!
bash: /sys/devices/platform/ocp/ocp*P9_31_pinmux/state: No such file or directory
Cannot write pinmux file: /sys/devices/platform/ocp/ocp*P9_31_pinmux/state
```
Solution: 
1. `sudo vim /boot/uEnv.txt`.
2. uncomment the line `disable_uboot_overlay_video=1`

* Install the `prussd.py` Daemon.<br>
Make sure that the proper version of prussd.py is running. Disable any previous prussd service:<br>
1. `sudo systemctl stop prussd.service`<br>
   `sudo systemctl disable prussd.service`<br>
2. `sudo bash install.sh --service`<br>
   `sudo systemctl start prussd.service`<br>
   `systemctl status prussd.service`

* Compiling the `pruss_api.c` driver: <br>
1. `sudo apt-get update`
2. `apt-cache search linux-headers-$(uname -r)`
3. `sudo apt-get install linux-headers-4.14.71-ti-r80`: Install the linux-headers-$(uname -r) for your kernel.
4. The linux headers should now be present in the `/usr/src/` directory.
5. Now, `cd drivers && make`
6. `sudo insmod pruss_api.ko`

* Make sure that proper paths and symbolic links have been made for clpru and lnkpru.<br>
The PRU compiler and linker are already installed on the standard images. They are called clpru and lnkpru.<br>
1. `export PRU_CGT=/usr/share/ti/cgt-pru`
2. `cd $PRU_CGT`
3. `mkdir -p bin`
4. `cd bin`
5. `ln -s `which clpru`  .`
6. `ln -s `which lnkpru` .`
Refer [https://zeekhuge.me/post/ptp_blinky/](https://zeekhuge.me/post/ptp_blinky/) and [https://groups.google.com/forum/#!topic/beagleboard/MBmIm0EnNfc](https://groups.google.com/forum/#!topic/beagleboard/MBmIm0EnNfc)

* [RPMsg Guide](https://github.com/pratimugale/PRUSS-Bindings/blob/master/Documentation/RPMsg.md)

* [Using SWIG to generate wrapper files.](https://github.com/pratimugale/PRUSS-Bindings/blob/master/Documentation/SWIG.md)
