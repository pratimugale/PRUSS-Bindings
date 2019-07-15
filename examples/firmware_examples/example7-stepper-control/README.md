# Stepper Motor Control

## Hardware Used

1. Motor: [NEMA 17 Motor]()
2. Driver: [EasyDriver A3967](https://robu.in/product/a3967-stepper-motor-driver-arduino/?gclid=CjwKCAjwgqbpBRAREiwAF046JYSdPNtmWnLB-B5GdAOZVuz6yiG3S-pwGJytae9ZFsmyN62aE_GSYxoCx_8QAvD_BwE)
3. BeagleBone: Black Rev C - Should work on other boards too with kernel v4.14


## Installation
1. Make sure that the proper symbolic links have been made for `clpru` and `lnkpru`. 
2. uncomment `disable_uboot_overlay_video=1` in `/boot/uEnv.txt`: This is to enable PRUs control over the GPIOs.
3. Give appropriate path the PRU_CGT env variable. (For compilation)
4. run `sudo make` in the directory. This will compile the code for both the PRUs.
5. Then compile the userspace .cpp program and run it.

## Setting Values

Explanation for `eighth step` mode i.e. 1.8 degree being broken into 8 parts - less torque output.
1. Each pulse provided rotates motor by 1 step.
2. 1 step = (1.8/8) degrees. Therefore 360 degrees -> ((360 * 8)/1.8) steps
3. i.e 1 rotation = 1600 steps.
4. Let default RPM as set in constructor be 120. -> 120 * 1600 steps in 60 seconds -> 3200 steps in 1 second.
5. Therefore default pulse frequency (used by PRU) = 3200Hz for 120 RPM

From the above calculation, we draw an important conclusion that is used in driver_lib.cpp:
60 RPM -> 1/8th step mode -> 1600 Hz
60 RPM -> 1/4th step mode ->  800 Hz 
60 RPM -> 1/4th step mode ->  400 Hz 
60 RPM -> 1/4th step mode ->  200 Hz 
So we can set the multipliers according to these values.

The largest number of pulses that can be sent to the motor is equal to the size of an unsigned integer i.e. 2^32.
