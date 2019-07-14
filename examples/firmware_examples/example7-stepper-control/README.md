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
