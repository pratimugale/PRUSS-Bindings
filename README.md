# PRUSS Bindings - An API to control the BBB PRUs
[![Build Status](https://travis-ci.org/pratimugale/PRUSS-Bindings.svg?branch=master)](https://travis-ci.org/pratimugale/PRUSS-Bindings)<br>

An API across multiple programming languages to use with the PRUs to load binaries, start/stop, and communicate with the PRUs from the ARM userspace.<br>
An Introductory Video describing the project: [https://www.youtube.com/watch?v=3Z2PxDIoCpE&t=5s](https://www.youtube.com/watch?v=3Z2PxDIoCpE&t=5s)<br>
An example made using the API: [https://www.youtube.com/watch?v=W-Kr37lqM98](https://www.youtube.com/watch?v=W-Kr37lqM98)

## Overview
The `cpp-bindings` interact with the Python Daemon Service through a UNIX Domain Socket file at `/tmp/pruss.sock` of the Linux file system.<br>
The `cpp-bindings` passes the appropriate request to the daemon through the socket file. The `prussd.py` daemon performs the required PRU-related task with root permissions and sends back the return value. <br>
The bindings for other scripting languages can then be built upon the cpp-bindings using SWIG, which takes C++ declarations and creates wrappers needed to access those declarations from other languages. Bindings have also been provided for 'C' language.<br>

![Workflow](./Documentation/workflow.jpg?raw=true)

## Contents
* `cpp-bindings/` : <br>
  The API which contains the C++ bindings to control the PRUs using prussd.py
* `prussd/` : <br>
  The Directory which contains the files related to the daemon which will run as a system service and serve PRU requests.
  This code does the actual work of controlling the PRUs.
* `examples/` : <br>
  The Directory which contains the example codes using the bindings. Contains firmware examples as well.
    * `example1/`: A simple LED Blinky example.
    * `example2-rpmsg-pru1/`: RPMsg Loopback - simplest RPMsg firmware for testing.
    * `example3-pwm/`: A PWM generator maximum frequency of about 1MHz.
    * `example4-analog-wave-gen/`: A PWM generator maximum frequency of about 1MHz.
    * `example5-multichannel-pwm/`: Multi-channel PWM based on the example given in PRU Cookbook - Accuracy needs to be improved.
    * `example6-memory-debug/`: To read/write to PRU SRAM/DRAM using /dev/mem and to demonstrate showRegs.
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
* `c-bindings/` : <br>
  The Directory which contains the C bindings for the PRUs.
* `python` : <br>
  The Directory which contains the PyPRUSS project (Python Bindings for the PRUs)[Discontinued]

## Installing and Using the API

There are two ways for installation:-
1. Download the packaged .deb file from [releases](https://github.com/pratimugale/PRUSS-Bindings/releases).<br>
   Run `dpkg -i pruapi_1.0-1_armhf.deb`
2. 1. Clone this repository: <br>
      `git clone https://github.com/pratimugale/PRUSS-Bindings`
   2. Run the install script<br>
      `cd PRUSS-Bindings/`<br>
      `bash install.sh`
   3. The API is now ready to use, run any specific example from `PRUSS-Bindings/examples/firmware_examples/` by `cd`'ing into the directory and running `make`. The Makefile will compile the PRU-firmware, load them on to the PRU(using /lib/firmware), compile the userspace program and run it. 

Make sure that RPMsg is working, here's a [guide](https://github.com/pratimugale/PRUSS-Bindings/blob/master/Documentation/RPMsg.md) for it.

### How to use the API?
After installation, this is how a simple userspace program looks like:
```cpp 
#include <iostream>
#include <pruss.h>

using namespace std;
int main()
{
	PRUSS& p = PRUSS::get();
	PRU p1 = p.pru1;
	if(!p1.load("/home/debian/gsoc/PRUSS-Bindings/examples/firmware_examples/example2-rpmsg-pru1/PRU1/am335x-pru1-fw"))
		cout << "Firmware loaded\n";
        else
                return -1;
	p1.enable();
	string s;
	cout << "Enter a message   : ";
	getline(cin, s);
	p1.sendMsg_string(s);
	cout << "Message from PRU  : "<< p1.getMsg();
	p1.disable();
	p.shutDown();
	return 0;

}
```
If installation is done from the debian package, compile using:
```
g++ userspace.cpp -L/usr/lib -lpruss
```

If installed from source, the processor directive must be `#include "path/to/cpp-bindings/pruss.h"`. Run this program by:
```
g++ userspace.cpp /path/to/cpp-bindings/pruss.cpp -o userspace.o
./userspace.o
```


## Notes
### `config-pin`: requires changes in uEnv.txt <br>
HDMI should be disabled. Otherwise this error is encountered:<br>
```
P9_31 pinmux file not found!
bash: /sys/devices/platform/ocp/ocp*P9_31_pinmux/state: No such file or directory
Cannot write pinmux file: /sys/devices/platform/ocp/ocp*P9_31_pinmux/state
```
Solution: 
1. `sudo vim /boot/uEnv.txt`.
2. uncomment the line `disable_uboot_overlay_video=1`

### Make sure that proper paths and symbolic links have been made for clpru and lnkpru.<br>
The PRU compiler and linker are already installed on the standard images. They are called clpru and lnkpru.<br>
1. `export PRU_CGT=/usr/share/ti/cgt-pru`
2. `cd $PRU_CGT`
3. `mkdir -p bin`
4. `cd bin`
5. ln -s which clpru  .
6. ln -s which lnkpru . <br>
Refer [https://zeekhuge.me/post/ptp_blinky/](https://zeekhuge.me/post/ptp_blinky/) and [https://groups.google.com/forum/#!topic/beagleboard/MBmIm0EnNfc](https://groups.google.com/forum/#!topic/beagleboard/MBmIm0EnNfc)

### RPMsg Guide
[/Documentation/RPMsg.md](./Documentation/RPMsg.md)

### Using SWIG to generate wrapper files.
[/Documentation/SWIG.md](./Documentation/SWIG.md)
