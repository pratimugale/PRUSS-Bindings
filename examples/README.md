# Examples

# Instructions to Run

* Firstly, clone the repository, this is a fork of Mohammed Muneeb's repository.
```
git clone https://github.com/pratimugale/PRUSS-Bindings
```
* Run the install script 
```
cd PRUSSS-Bindings/
bash install.sh
```

* `cd` into the specific example in the `firmware_examples/` directory and run `make`.

**NOTE**: The code in `archive_userspace_programs` and `/firmware_examples/archive_firmware` were made by a previous student and are not maintained by me. <br>
To run them, theses are the instructions:

```
bash install.sh --examples
```
Then, 
* Run the specific example

**Blinky**

Connect an LED on pin 8.46 through a >470 Ohm resistor.
```
./blinky.o
```
The LED should start blinking. you can press `p` to pause, `r` to resume, `d` to disable and `e` to enable the PRU.

**RPMsg Echo**

This example demonstrates the use of the RPMsg functions. The message entered is sent to the PRU and the PRU sends it back to the user.
```
/rpmsg_echo.o
```
**Pin State Reader**

This example demonstrates the use of the event wait functions. The PRU monitors the pin 8.45 for a state change and sends a message to the PRU when the state of the pin is changed.
```
./pru_pin_state_reader.o
```
The program will wait for the PRU on the channel. Connect a wire from the 3.3V pin (eg P9.3) to the pin 8.45 and you should see 'Event occured' displayed. You can also specify the timeout after which a 'Timeout Occured' will be displayed.

**Inter-PRU to ARM communication**

The PRU1 sends an interrupt to PRU0 which then sends a message 'Interrupted' to its rpmsg channel every second.
```
./pru1_to_pru0_to_arm.o
```

**Multi Threaded Demo**

Similar to the previous example, but demonstrating the multithreaded nature of the daemon server. One thread is waiting on the rpmsg channel for messages and the other thread controls the PRU1 (pauses/resumes it).
```
./multi_threaded.o
```
Now one thread is waiting on the channel, whereas other thread asks for input PRU1 is paused, so no interrupts are send. Press any key and press enter to resume PRU1 and you can see the message 'Interrupted' showing that the message has been recieved on the channel.
