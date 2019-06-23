# PWM Example

This example generates a PWM output of a desired voltage at the pin P9_31.<br>
`config-pin P9_31 pruout`<br>
`disable_uboot_overlay_video=1` in `/boot/uEnv.txt`<br>
`export PRUN=0`<br>
`export TARGET=pwm_test`<br>
`export PRU_CGT=/usr/share/ti/cgt-pru`<br>
`make clean && make`

Sometimes the PRU pin gives out the wrong voltage. In this case, rebooting the BBB and re-compiling the firmware should solve it.<br>
running the `config-pin P9_31 pruout` AFTER the `make` command also works.
