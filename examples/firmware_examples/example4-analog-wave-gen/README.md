# Analog Wave Generator

The PWM code can be modified to generate analog-like waveforms by altering the PWM duty-cycle rapidly over time. This is because the PWM frequency can be very fast due to the 200 MHz PRU. The code will output any periodic waveform that is passed to it, with a maximum periodic sample length of about 12KB(PRU SRAM) in this example. The program can be stopped by echoing stop in the appropriate sysfs entry(I'm not using a button interrupt to stop the execution). 












