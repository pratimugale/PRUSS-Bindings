# High Level API Specification

![UML Diagram](https://raw.githubusercontent.com/MuneebMohammed/PRUSS-Bindings/master/Documentation/uml.png)
 
## `class PRUSS`
The parent class which encapsulates the PRU-ICSS on the Beaglebone. It is a composition of two instances of the `PRU` class.
### `PRUSS()`
Constructor. Creates an instance of the `PRUSS` class. Boots up the Remote Processors and enables the remoteproc drivers. Also instantiates two `PRU` objects.
### `void isOn()`
Returns a boolean specifying whether the PRUSS is booted or not.
### `void bootUp()`
Enables the Drivers and boots up the System
### `void shutDown()`
Shuts down the PRU-ICSS by disabling the RemoteProc drivers.
### `void pinMode( int header, int pin, Boolean out)`
Configures the pinmux settings for the pin specified. If universal cape not enabled, it will build and load a Device Tree Overlay(DTO) to change the pinmux settings.

**Parameters**
* `header` - 8/9
* `pin` - The pin on the header
* `out` - Whether the pin should be pruout or pruin
### `void writeSharedMem(int data, int offset)`
Writes 4 bytes of data to the PRU Shared Memory (12 KB) with the given address offset.

**Parameters**
* `data` - the data to be written
* `offset` - the address offset to which the data has to be written

### `void readSharedMem(int offset)`
Read 4 bytes of data from the PRU Shared Memory at the address address offset.

**Parameters**
* `offset` - the address offset from which the data is to be read.

## `class PRU`
The class which encapsulates a PRU Core in the PRU-ICSS.
### `PRU(int number)`
Constructor. Creates an instance of `PRU` with the specified Core number (can be 0 or 1 only) and enables the PRU Core.

**Parameters**
* `number` - the PRU core number - 0/1

### `PRU(int number, String fw)`
Constructor. Creates an instance of `PRU` with the specified Core number and also loads the firmware image specified by `fw`

**Parameters**
* `number` - the PRU core number - 0/1
* `fw` - the firware binary to be loaded onto the PRU Core

### `void enable()`
Enables the PRU. Changes the PRU `state` to `running`.

### `void disable()`
Disables the PRU. Changes the PRU `state` to `stopped`.

### `void reset()`
Disables and then Enables the PRU Core.

### `void pause()`
Halts the PRU Execution if its `running`. Changes the `state` to `halted`.

### `void resume()`
Resumes the PRU Execution it its `halted`.

### `void showRegs()`
Prints the current values of the PRU Control Registers and PRU General Purpose Registers(only if the PRU is not `running`) for debugging purposes.

### `void load(String fw)`
Loads the specified firmware image file `fw` onto the PRU Core and runs it.

**Parameters**
* `fw` - the firmware image to be loaded. Cannot be `NULL`.

### `void setChannel([int channelNo])`
Sets the RPMsg `channel` of the PRU Core to the one corresponding to `channelNo`. If not specified, the `channelNo` defaults to 30 and 31 for PRU `number` 0 and 1 respectively.

**Parameters**
* `channelNo` - The Channel number of the RPMsg channel as specified in the firmware, during channel creation.

### `State getState()`
Returns the current `state` of the PRU.

### `void sendMsg(String message)`
Sends a `message` to the PRU over its RPMsg `channel`.

**Parameters**
* `message` - The message to be sent.

### `String getMsg()`
Returns a message from the PRU, available on its RPMsg `channel`. Returns `NULL` if no message found.

### `void waitForEvent()`
Wait for a signal from the PRU. Will stall with a blocking read until a signal is recieved on the PRU's `channel`.

### `void writeDataMem(int data, int offset)`
Writes 4 bytes of data to the respective Data Memory with the given address offset.

**Parameters**
* `data` - the data to be written
* `offset` - the address offset to which the data has to be written

### `void readSharedMem(int offset)`
Read 4 bytes of data from the respective Data Memory at the address address offset.

**Parameters**
* `offset` - the address offset from which the data is to be read.

## `enum State`
An Enumeration which describes the state of the PRU Core.
* `none` - the PRUSS is shutdown (and so is the PRU).
* `offline` - the PRU Core is disabled/stopped.
* `running` - the PRU Core is enabled and in execution.
* `halted` - the PRU Core is halted or paused (by `pause()` function)
