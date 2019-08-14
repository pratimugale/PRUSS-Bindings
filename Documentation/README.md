# High Level API Specification

![UML Diagram](https://raw.githubusercontent.com/pratimugale/PRUSS-Bindings/master/Documentation/cpp-bindings-uml.jpg)
 
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
### `void restart()`
Restarts the PRU-ICSS, essentially shuts down the PRU-ICSS and boots it up again.

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

### `int sendMsg_string(String message)`
Sends a `message` in string(character) form to the PRU over its RPMsg `channel`. Returns `0` if successful, else returns the corresponding Error Code.

**Parameters**
* `message` - The message to be sent.

### `int sendMsg_raw(String message)`
Sends a `message` in its raw(binary) form to the PRU over its RPMsg `channel`. Returns `0` if successful, else returns the corresponding Error Code.<br>
The raw `message`, suppose in integer form, still needs to be passed to this function as a string.

**Parameters**
* `message` - The raw message to be sent.

### `String getMsg()`
Returns a message from the PRU, available on its RPMsg `channel`. Returns `NULL` if no message found.

### `int waitForEvent([int timeout])`
Wait for a signal from the PRU. Will stall with a blocking read until a signal is recieved on the PRU's `channel` or until `timeout`. Returns `0` if successful, else returns the corresponding Error Code.

**Parameters**
* `timeout` - optional argument, specifies the maximum time (in milliseconds) the function will wait. Will return ETIMEDOUT error code after `timeout`. If not specified, blocks indefinitely till the signal is recieved.

### `int mem_write(Memory mem, string offset, string data)`
Writes 1 byte of data to the respective Memory (check `enum Memory`) with the given address offset. Returns `0` if successful, else returns the corresponding Error Code.

**Parameters**
* `mem` - denotes which memory block is to be used: PRU0 Data Memory, PRU1 Data Memory or the PRU Shared Memory. Integer values specified in `enum Memory`
* `offset` - the address offset to which the data has to be written
* `data` - the data to be written

### `string mem_read(Memory mem, string offset)`
Read 1 byte of data from the respective Memory at the address `offset`. Returns the data read (as a string) if successful, else returns the negative of the corresponding Error Code.

**Parameters**
* `mem` - denotes which memory block is to be used: PRU0 Data Memory, PRU1 Data Memory or the PRU Shared Memory. Integer values specified in `enum Memory`
* `offset` - the address offset from which the data is to be read.

## `class Socket`
The class which encapsulates the Client socket functions for Unix Domain Sockets. The instances of this class are used by the `PRUSS`  and the `PRU` classes to send commands to the Socket Server to request for PRU Resources.
### `Socket()`
Constructor. Creates an instance of the Socket Class. Initialises the `socketpath` to `/tmp/prusocket` which is the default socket file for PRU requests.
### `Boolean conn()`
Opens a socket connection. Returns `true` if successful and `false` otherwise.
### `Boolean disconn()`
Disconnects from the socket connection. Returns `true` if successful and `false` otherwise.
### `String sendcmd(String command)`
Sends a command to the server over the socket connection. Returns the reply send by the server over the socket connection.

**Parameters**
* `command` - the command to be sent to the socket server

## `enum State`
An Enumeration which describes the state of the PRU Core.
* `none` - the PRUSS is shutdown (and so is the PRU).
* `offline` - the PRU Core is disabled/stopped.
* `running` - the PRU Core is enabled and in execution.
* `halted` - the PRU Core is halted or paused (by `pause()` function)

## `enum Memory`
An Enumeration which describes which memory block is to be used during the read/write operations.
* `DATA0 = 0` - Memory Read/Write to be done on PRU Data Memory 0
* `DATA1 = 1` - Memory Read/Write to be done on PRU Data Memory 1
* `SHARED = 3` - Memory Read/Write to be done on PRU Shared Memory 
