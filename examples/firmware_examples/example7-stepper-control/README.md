# Stepper Motor Control

Video of working: [https://www.youtube.com/watch?v=joPQ1JjVkvU](https://www.youtube.com/watch?v=joPQ1JjVkvU)
Will add a demonstration video soon.

## Hardware Used

1. Motor: [NEMA 17 Motor]()
2. Driver: [EasyDriver A3967](https://robu.in/product/a3967-stepper-motor-driver-arduino/?gclid=CjwKCAjwgqbpBRAREiwAF046JYSdPNtmWnLB-B5GdAOZVuz6yiG3S-pwGJytae9ZFsmyN62aE_GSYxoCx_8QAvD_BwE)
3. BeagleBone: Black Rev C - Should work on other boards too with kernel v4.14


## User Documentation
1. Make sure that the proper symbolic links have been made for `clpru` and `lnkpru`. 
2. uncomment `disable_uboot_overlay_video=1` in `/boot/uEnv.txt`: This is to enable PRUs control over the GPIOs.
3. Give appropriate path the PRU_CGT env variable. (For compilation)
4. Make sure that RPMsg is set up and is working - A guide can be found in the main Documentation directory.
5. Set the motor commands in `user_test.cpp`
6. run `sudo make` in this directory. This will compile the code for both the PRUs.
7. It will also compile the userspace: user_test.cpp program and run it.

## Technical Documentation
1. `/PRU0`: Firmware for PRU0:<br>
   PRU0 reads 3 4-byte values from the PRU SRAM at address offsets: 0x00010000, 0x00010004, 0x00010008 <br>
    1. `/PRU0/main_pru.c` - Configures interrupts and starts `pulse-supply.asm`
    2. `/PRU0/pulse-supply.asm` - Sends out precise amount of pulses to the motor/driver at the required frequency(RPM). Interrupts PRU1 after it is done.
2. `/PRU1`: Firmware for PRU1:<br>
   PRU1 takes input frequency, number of pulses from the user on the channel `rpmsg-pru31` <br>
    1. `/PRU1/main.c`: Sets up RPMsg, stores input to PRU SRAM address offsets mentioned above. Then waits for interrupt from PRU0 after which it sends back "done\n" to the same rpmsg channel. 
3. `/driver_lib.cpp`: Library to perform stepper motor tasks, starts/stops the PRUs, calculates onCycles, totalCycles, noOfCycles and sends it to PRU1.
4. `user_test.cpp`: Uses driver_lib.cpp to activate the motor.

## Setting Values
Explanation for `eighth step` mode i.e. 1.8 degree being broken into 8 parts - less torque output.
1. Each pulse provided rotates motor by 1 step.
2. 1 step = (1.8/8) degrees. Therefore 360 degrees -> ((360 * 8)/1.8) steps
3. i.e 1 rotation = 1600 steps.
4. Let default RPM as set in constructor be 120. -> 120 * 1600 steps in 60 seconds -> 3200 steps in 1 second.
5. Therefore default pulse frequency (used by PRU) = 3200Hz for 120 RPM

From the above calculation, we draw an important conclusion that is used in driver_lib.cpp:<br>
60 RPM -> 1/8th step mode -> 1600 Hz <br>
60 RPM -> 1/4th step mode ->  800 Hz <br>
60 RPM -> 1/4th step mode ->  400 Hz <br>
60 RPM -> 1/4th step mode ->  200 Hz <br>
So we can set the multipliers according to these values.<br>

The largest number of pulses that can be sent to the motor is equal to the size of an unsigned integer i.e. 2^32.


