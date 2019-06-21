# PRUSS Bindings
Language Bindings to interact with the PRUs on the Beaglebone.

## Overview
The cpp-bindings interact with the Python Daemon Service through UNIX Domain Sockets.
cpp-bindings contains functions that the userspace program will use; the cpp-bindings would then pass the appropriate request to the daemon through the socket file. The daemon performs the required task with the PRU and sends back the return value. The advantage of having a daemon is that userspace apps don't need to deal with root permissions, although restricting access to the socket is necessary.
The bindings for other scripting languages can then be built upon the cpp-bindings using SWIG, which generates wrapper functions for C/C++ programs. Bindings have also been provided for 'C' language in case the user wants to work with C instead of using OOP.

## Description
* `prussd` : The Directory which contains the files related to the daemon which will run as a system service and serve PRU requests.
* `cpp-bindings` : The Directory which contains the C++ bindings using the prussd daemon service (therefore using the PRUs without root access)
* `examples` : The directory which contains the example codes using the bindings. Contains firmware examples as well.
* `swig` : The Directory which contains the swig config and interface files
* `Documentation` : Contains the API Documentation/Specification
* `install.sh` : The install script for the project
* `python` : The Directory which contains the PyPRUSS project (Python Bindings for the PRUs)[Discontinued]
