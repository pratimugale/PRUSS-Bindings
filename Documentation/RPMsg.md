# RPMsg

This document explains how to run TI's lab_5 (and similarly any other RPMsg firmware example) on the BBB, without using the Code Composer Studio.
This method is working as of 26-06-2019.
RPMsg is a little tricky to get started with so I recommend following these steps. These are the exhaustive steps in using RPMsgs: 

1. SSH into the BeagleBone Black.
2. Some tasks need you to be the root user, so login as root or use `sudo` if you get `permission denied` error.
3. `$ uname -r` to verify kernel -> mine at the moment is _4.14.71-ti-r80_ <br>
Debian Version: BeagleBoard.org Debian Image 2018-10-07 (use `$ cat /etc/issue` to find)
4. `$ sudo apt-get update`.
5. cd / and then find clpru compiler which is generally in /usr/share/ti/cgt-pru. This is the location of the PRU code generation tools - compiler.
6. On doing this: 
```
$ which clpru
/usr/bin/clpru
``` 
So the compiler binary is in a different location. This is a problem for labs to make files.<br>
```
$ cd /usr/share/ti/cgt-pru
$ mkdir bin
$ ln -s /usr/bin/clpru clpru
```
So now the make files will find the compiler executable in the correct location via the symbolic link.

 *ARM Linux also needs to create a symbolic link to the /usr/bin/ directory in
 order to use the same Makefile
(ARM Linux) ln -s /usr/bin/ /usr/share/ti/cgt-pru/bin* - I'm not entirely sure about this but I'll check on a different image and update.

7. Make sure all the modules are loaded.<br>
```
$ sudo modprobe pru_rproc
$ sudo modprobe rpmsg_pru
$ sudo modprobe virtio_rpmsg_bus
```
8. `$ cd ~`<br>
`$ git clone git://git.ti.com/pru-software-support-package/pru-software-support-package.git`<br>
This will clone a copy of the latest pru-software-support-package: The ones used in TI's Hands On Training Labs.
9. cd into _lab_5_ in the package:
```
$ cd lab_5/solution/PRU_Halt
$ make
```
This will fail, it is looking for the environment variable $PRU_CGT<br>
`$ export PRU_CGT=/usr/share/ti/cgt-pru`<br>
Now try _make_ again. It should succeed.<br>

10. Do (for **pru0**)
```
$ cd gen
$ sudo cp PRU_Halt.out am335x-pru0-fw
$ sudo cp am335x-pru0-fw /lib/firmware
```

11. Now cd into the PRU_RPMsg_Echo_Interrupt1 directory in the same lab_5.<br>
Edit main.c as follows:
```
//#define CHAN_NAME                 "rpmsg-client-sample"
#define CHAN_NAME                   "rpmsg-pru"
```
(Comment, Uncomment)

12. Now almost the same as #10, this time for **pru1**:
```
$ cd gen
$ cp PRU_RPMsg_Echo_Interrupt1.out am335x-pru1-fw
$ cp am335x-pru1-fw /lib/firmware
```

13. (i)***REBOOT THE BBB (sudo reboot)***, This is important otherwise you will get `state not found` error; (ii) Start the second PRU i.e. pru1 -
`$ echo start | sudo tee /sys/class/remoteproc/remoteproc2/state`<br>

14. _cd /dev_ look for _rpmsg_pru31_ device file.  It will be there! <br>

15. `$ dmesg` or `$ dmesg | grep pru` - The last few lines of dmesg should be something like this:
```
[   76.778231] pruss 4a300000.pruss: creating PRU cores and other child platform devices
[   76.811666] remoteproc remoteproc1: 4a334000.pru is available
[   76.811792] pru-rproc 4a334000.pru: PRU rproc node /ocp/pruss_soc_bus@4a326004/pruss@0/pru@34000 probed successfully
[   76.813392] remoteproc remoteproc2: 4a338000.pru is available
[   76.813498] pru-rproc 4a338000.pru: PRU rproc node /ocp/pruss_soc_bus@4a326004/pruss@0/pru@38000 probed successfully
[ 2035.360460] remoteproc remoteproc2: powering up 4a338000.pru
[ 2035.371491] remoteproc remoteproc2: Booting fw image am335x-pru1-fw, size 74032
[ 2035.374631] pruss 4a300000.pruss: configured system_events[63-0] = 0x00000000.000c0000
[ 2035.374652] pruss 4a300000.pruss: configured intr_channels = 0x0000000a host_intr = 0x0000000a
[ 2035.383501] virtio_rpmsg_bus virtio0: creating channel rpmsg-pru addr 0x1f
[ 2035.384526] rpmsg_pru virtio0.rpmsg-pru.-1.31: new rpmsg_pru device: /dev/rpmsg_pru31
[ 2035.385431] virtio_rpmsg_bus virtio0: rpmsg host is online
[ 2035.385594] remoteproc remoteproc2: registered virtio0 (type 7)
[ 2035.385605] remoteproc remoteproc2: remote processor 4a338000.pru is now up
```

16. Now that the firmware has been loaded and the device file is ready, we need to echo into this file and read its output to communicate with it.<br>
Go back to pru_software_support_package/labs/lab5

17. Compile the `rpmsg_pru_user_space_echo.c` file using GCC compiler (`$ g++ rpmsg_pru_user_space_echo.c`).<br>

18. **`$ sudo ./a.out` MAKE SURE YOU ARE THE SUPERUSER WHILE EXECUTING ./a.out OTHERWISE YOU WILL GET `"Failed to open /dev/rpmsg31"` ERROR**

19. You sould be able to execute the file and get "Received 100 messages, closing /dev/rpmsg31" as output.


Hopefully I did not miss any of the steps.  Let me know if any changes are needed.

## Additional Notes
1. _The entry **remoteproc0** relates to the Wakeup M3 (CM3) remoteproc driver that helps with low power tasks on the Cortex M3 co-processor in the AM33xx family of devices-it has no role in controlling the PRU-ICSS._
2. **"main.c", line 72: error #20: identifier "RPMSG_MESSAGE_SIZE" is undefined** - If you are getting this error, change the line (72 in this case) from `uint8_t payload[RPMSG_MESSAGE_SIZE]`; to `uint8_t payload[50]`.

## Sources: 
Greg's answer: [https://groups.google.com/d/msg/beagleboard/od6h9yTKUD4/jzGE6KaxAQAJ](https://groups.google.com/d/msg/beagleboard/od6h9yTKUD4/jzGE6KaxAQAJ)<br>
Abhishek's answer: [https://groups.google.com/d/msg/beagleboard/4P9NdglojBo/qqizuGCZAQAJ](https://groups.google.com/d/msg/beagleboard/4P9NdglojBo/qqizuGCZAQAJ])

