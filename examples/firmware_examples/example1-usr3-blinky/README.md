# Blinky example

This example makes the USR3 LED on the BeagleBone Black to blink 5 times.
The example is a minimal one which should demonstrate how the PRU firmware looks like. 

There are 4 essential files required for one PRU core while using the Remoteproc Framework:

1. The `AM335x_PRU.cmd` linker command file.
2. A resource table 
3. The actual firmware written in C (can also use assemby)
4. The `Makefile` which uses `clpru` and `lnkpru` to compile the firmware using the above 3 files.

The Makefile generates a number of files of which the `.out` elf binary file needs to be placed in `/lib/firmware` as `am335x-pru0-fw` or `am335x-pru1-fw` depending upon which core the firmware is meant for.

Once placed in /lib/firmware, the PRU needs to be started by echoing into the appropriate sysfs entry. One such option is 
- `/sys/devices/platform/ocp/4a32600*.pruss-soc-bus/4a300000.pruss/$(PRU_ADDR).*/remoteproc/remoteproc*)`


To debug the PRU or to use it in single_step mode, use:<br> `/sys/kernel/debug/remoteproc/remoteproc1/` for PRU0 <br>`/sys/kernel/debug/remoteproc/remoteproc2/` for PRU0.

There is also a third entry called `remoteproc0` in the remoteproc directory. **IT HAS NO ROLE IN CONTROLLING THE PRU-ICSS**. It relates to the Wakeup M3 (CM3) remoteproc driver that helps with low power tasks on the Cortex M3 co-processor in the AM33xx family of devices.

To echo into these sysfs entries, `echo 1 > single_step` no longer works (I have seen this mentioned on the net). The reason being when you run echo with sudo the output is redirected by your command shell which hasn't got the privileges. One way around this would be :<br> `echo 1 | sudo tee single_step`, <br> or something like `sudo sh -c "echo out > /sys/class/gpio/gpio49/direction"`

Note: The bind/unbind interfaces that were used before are no longer used and don't work.
