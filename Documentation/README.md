# High Level API Specification

![UML Diagram](https://raw.githubusercontent.com/MuneebMohammed/PRUSS-Bindings/master/Documentation/uml.png)
 
## `class PRUSS`
The parent class which encapsulates the PRU-ICSS on the Beaglebone. It is a composition of two instances of the `PRU` class.
### `PRUSS()`
Constructor. Creates an instance of the `PRUSS` class. Boots up the Remote Processors and enables the remoteproc drivers. Also instantiates two `PRU` objects.
### `Boolean isOn()`
Returns a boolean specifying whether the PRUSS is booted or not.
### `int bootUp()`
Enables the Drivers and boots up the System. Returns `0` if successful, else returns the corresponding Error Code.
### `int shutDown()`
Shuts down the PRU-ICSS by disabling the RemoteProc drivers. Returns `0` if successful, else return the corresponding Error Code
### `int pinMode( int header, int pin, Boolean out)`
Configures the pinmux settings for the pin specified. Returns `0` if successful, else returns the corresponding Error Code.

**Parameters**
* `header` - 8/9
* `pin` - The pin on the header
* `out` - Whether the pin should be pruout or pruin
### `int writeSharedMem(int data, int offset)`
Writes 4 bytes of data to the PRU Shared Memory (12 KB) with the given address offset. Returns `0` if successful, else returns the corresponding Error Code.

**Parameters**
* `data` - the data to be written
* `offset` - the address offset to which the data has to be written

### `int readSharedMem(int offset)`
Read 4 bytes of data from the PRU Shared Memory at the address `offset`. Returns the data read (a positive int) if successful, else returns the negative of the Error Code.

**Parameters**
* `offset` - the address offset from which the data is to be read.

## `class PRU`
The class which encapsulates a PRU Core in the PRU-ICSS.
### `PRU(int number)`
Constructor. Creates an instance of `PRU` with the specified Core number (can be 0 or 1 only) and enables the PRU Core.

**Parameters**
* `number` - the PRU core number - 0/1

### `PRU(int number, String fw)`
Constructor. Creates an instance of `PRU` with the specified Core number and also loads the firmware image specified by `fw`.

**Parameters**
* `number` - the PRU core number - 0/1
* `fw` - the firware binary (along with the full path) to be loaded onto the PRU Core.

### `int enable()`
Enables the PRU. Changes the PRU `state` to `running`. Returns `0` if successful, else returns the corresponding Error Code.

### `int disable()`
Disables the PRU. Changes the PRU `state` to `stopped`. Returns `0` if successful, else returns the corresponding Error Code.

### `int reset()`
Disables and then Enables the PRU Core. Returns `0` if successful, else returns the corresponding Error Code.

### `int pause()`
Halts the PRU Execution if it's `running`. Changes the `state` to `halted`. Returns `0` if successful, else returns the corresponding Error Code.

### `int resume()`
Resumes the PRU Execution if it's `halted`. Returns `0` if successful, else returns the corresponding Error Code.

### `int showRegs()`
Prints the current values of the PRU Control Registers and PRU General Purpose Registers(only if the PRU is not `running`) for debugging purposes. Returns `0` if successful, else returns the corresponding Error Code.

### `int load(String fw)`
Loads the specified firmware image file `fw` onto the PRU Core and runs it. Returns `0` if successful, else returns the corresponding Error Code.

**Parameters**
* `fw` - the firmware image (along with the full path) to be loaded. Cannot be `NULL`.

### `int setChannel([int channelNo])`
Sets the RPMsg `channel` of the PRU Core to the one corresponding to `channelNo`. If not specified, the `channelNo` defaults to 30 and 31 for PRU `number` 0 and 1 respectively. Returns `0` if successful, else returns the corresponding Error Code.

**Parameters**
* `channelNo` - The Channel number of the RPMsg channel as specified in the firmware, during channel creation.

### `State getState()`
Returns the current `state` of the PRU.

### `int sendMsg(String message)`
Sends a `message` to the PRU over its RPMsg `channel`. Returns `0` if successful, else returns the corresponding Error Code.

**Parameters**
* `message` - The message to be sent.

### `String getMsg()`
Returns a message from the PRU, available on its RPMsg `channel`. Returns `NULL` if no message found.

### `int waitForEvent([int timeout])`
Wait for a signal from the PRU. Will stall with a blocking read until a signal is recieved on the PRU's `channel` or until `timeout`. Returns `0` if successful, else returns the corresponding Error Code.

**Parameters**
* `timeout` - optional argument, specifies the maximum time (in milliseconds) the function will wait. Will return ETIMEDOUT error code after `timeout`. If not specified, blocks indefinitely till the signal is recieved.

### `int writeDataMem(int data, int offset)`
Writes 4 bytes of data to the respective Data Memory with the given address offset. Returns `0` if successful, else returns the corresponding Error Code.

**Parameters**
* `data` - the data to be written
* `offset` - the address offset to which the data has to be written

### `int readDataMem(int offset)`
Read 4 bytes of data from the respective Data Memory at the address `offset`. Returns the data read (a positive integer) if successful, else returns the negative of the corresponding Error Code.

**Parameters**
* `offset` - the address offset from which the data is to be read.

## `enum State`
An Enumeration which describes the state of the PRU Core.
* `none` - the PRUSS is shutdown (and so is the PRU).
* `offline` - the PRU Core is disabled/stopped.
* `running` - the PRU Core is enabled and in execution.
* `halted` - the PRU Core is halted or paused (by `pause()` function)
