# PRUSS Bindings - An API to control the BBB PRUs
An API across multiple programming languages to use with the PRUs to load binaries, start/stop, and communicate with the PRUs from the ARM userspace.<br>
An Introductory Video describing the project: [https://www.youtube.com/watch?v=3Z2PxDIoCpE&t=5s](https://www.youtube.com/watch?v=3Z2PxDIoCpE&t=5s)<br>
Example of using the API: [https://www.youtube.com/watch?v=W-Kr37lqM98](https://www.youtube.com/watch?v=W-Kr37lqM98)

## Overview
The `cpp-bindings` interact with the Python Daemon Service through UNIX Domain Sockets.
cpp-bindings contains functions that the userspace program will use; the cpp-bindings would then pass the appropriate request to the daemon through the socket file (path: /tmp/prussd.sock). The daemon performs the required task with the PRU and sends back the return value. The advantage of having a daemon is that userspace apps don't need to deal with root permissions, although restricting access to the socket is necessary.
The bindings for other scripting languages can then be built upon the cpp-bindings using SWIG, which generates wrapper functions for C/C++ programs. Bindings have also been provided for 'C' language in case the user wants to work with C instead of using OOP.

## Contents
* `prussd/` : <br>
  The Directory which contains the files related to the daemon which will run as a system service and serve PRU requests.
* `cpp-bindings/` : <br>
  The Directory which contains the C++ bindings using the prussd daemon service (therefore using the PRUs without root access)
* `c-bindings/` : <br>
  The Directory which contains the C bindings using the prussd daemon service.
* `examples/` : <br>
  The directory which contains the example codes using the bindings. Contains firmware examples as well.
    * `example1/`: A simple LED Blinky example.
    * `example2-rpmsg-pru1/`: RPMsg Loopback - simplest RPMsg firmware for testing.
    * `example3-pwm/`: A PWM generator maximum frequency of about 1MHz.
    * `example4-analog-wave-gen/`: A PWM generator maximum frequency of about 1MHz.
    * `example5-multichannel-pwm/`: Multi-channel PWM based on the example given in PRU Cookbook - Accuracy needs to be improved.
    * `example6-memory-debug/`: To read/write to PRU SRAM/DRAM using /dev/mem.
    * `example7-stepper-control/`: A library to accurately control stepper motors which are driven by the PRU.
    * `example8-multiple-assembly-calls/`: To demonstrate how to link together multiple .asm files and call them from the C-program. Control is passed back to the C-program by using **R3.w2**
    * `example9-multichannel-waveform-gen/`: Expanding example4 to upto 8-channels.
* `swig` : <br>
  The Directory which contains the swig config and interface files
* `Documentation` : <br>
  Contains the API Documentation/Specification
* `install.sh` : <br>
  The install script for the project
* `drivers` : <br>
  A dedicated rpmsg driver for the bindings called `pruss_api`
* `python` : <br>
  The Directory which contains the PyPRUSS project (Python Bindings for the PRUs)[Discontinued]

## Installation Guide

### `config-pin`: requires changes in uEnv.txt - HDMI should be disabled. Otherwise this error is encountered:<br>
```
P9_31 pinmux file not found!
bash: /sys/devices/platform/ocp/ocp*P9_31_pinmux/state: No such file or directory
Cannot write pinmux file: /sys/devices/platform/ocp/ocp*P9_31_pinmux/state
```
Solution: 
1. `sudo vim /boot/uEnv.txt`.
2. uncomment the line `disable_uboot_overlay_video=1`

### Install the `prussd.py` Daemon.<br>
Make sure that the proper version of prussd.py is running. Disable any previous prussd service:<br>
1. `sudo systemctl stop prussd.service`<br>
   `sudo systemctl disable prussd.service`<br>
2. `sudo bash install.sh --service`<br>
   `sudo systemctl start prussd.service`<br>
   `systemctl status prussd.service`

### Compiling the `pruss_api.c` driver: <br>
1. `sudo apt-get update`
2. `apt-cache search linux-headers-$(uname -r)`
3. `sudo apt-get install linux-headers-4.14.71-ti-r80`: Install the linux-headers-$(uname -r) for your kernel.
4. The linux headers should now be present in the `/usr/src/` directory.
5. Now, `cd drivers && make`
6. `sudo insmod pruss_api.ko`

### Make sure that proper paths and symbolic links have been made for clpru and lnkpru.<br>
The PRU compiler and linker are already installed on the standard images. They are called clpru and lnkpru.<br>
1. `export PRU_CGT=/usr/share/ti/cgt-pru`
2. `cd $PRU_CGT`
3. `mkdir -p bin`
4. `cd bin`
5. `ln -s `which clpru`  .`
6. `ln -s `which lnkpru` .`
Refer [https://zeekhuge.me/post/ptp_blinky/](https://zeekhuge.me/post/ptp_blinky/) and [https://groups.google.com/forum/#!topic/beagleboard/MBmIm0EnNfc](https://groups.google.com/forum/#!topic/beagleboard/MBmIm0EnNfc)

### [RPMsg Guide](https://github.com/pratimugale/PRUSS-Bindings/blob/master/Documentation/RPMsg.md)

### [Using SWIG to generate wrapper files.](https://github.com/pratimugale/PRUSS-Bindings/blob/master/Documentation/SWIG.md)
