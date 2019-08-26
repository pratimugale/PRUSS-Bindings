#include "pruss.h"

using namespace std;


/**
 * Constructor for class Socket that establishes unix domain socket connection with prussd.py
 *
 * Sets the file descriptor to -1 initially and the name of the socket file over which commands are to be sent to the prussd.py daemon service
 */
Socket::Socket()
{
	this->fd = -1;
	this->socketpath = "/tmp/prussd.sock";
}

/**
 * Connect to socket file 
 *
 * @return true if connection to socket address is successful
 */
bool Socket::conn()
{
	
	// get a socket to work with. The socket will be a unix domain stream socket
	this->fd = socket(AF_UNIX, SOCK_STREAM, 0);
	
	//fill the socket address struct with family name and socket path
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(this->addr.sun_path, socketpath, sizeof(addr.sun_path)-1);
	
	//connect to the socket address
	if(connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
		return false;
	
	return true;
}

/**
 * Send command to prussd.py over /tmp/prussd.sock
 *
 * After the connection to the unix domain socket file has been established, this function is 
 * used to send instructions that the daemon should carry out.
 * Also used to communicate with the PRUs using RPMsg.
 *
 * @param command which should be in exaclty the same format as the prussd.py daemon knows
 * @return the message sent back by the PRU or appropriate error code if something doesn't work out. 0 if some command is carried out by the daemon successfully
 */
string Socket::sendcmd(string command)
{
	if(!this->conn()) // Connect to the socket 
		return to_string(ECONNREFUSED);
	
	int nbytes;
	string received;
	char buf[2048], rec[2048]; //buffers to store command and reply
	nbytes = snprintf(buf, sizeof(buf), command.c_str()); //store command in buf
	buf[nbytes] = '\n';
	send(this->fd, buf, strlen(buf), 0); // send command over the socket connection

	nbytes = recv(this->fd, rec, sizeof(rec), 0); // receive reply from server
	rec[nbytes] = '\0'; // string boundary
	received = std::string(rec); // convert char* to string
	
	this->disconn(); // disconnect from the socket connection
	
	return received;
}

/**
 * Disconnect from the socket connection
 *
 * Closes the socket connection with prussd.py over /tmp/prussd.sock
 *
 * @return true if socket was disconnected successfully, false if it was already disconnected.
 */
bool Socket::disconn()
{
	if(this->fd == -1)
		return false;
	close(this->fd);
	this->fd = -1;
	return true;
}

/**
 * Function that returns an object of type PRUSS to the user.
 *
 * @return object of type PRUSS
 */
PRUSS& PRUSS::get()
{
	static PRUSS p;
	return p;
}

/**
 * Constructor for PRUSS
 *
 * Stops both the PRUs and probes the remoteproc driver
 *
 */
PRUSS::PRUSS() : pru0(0), pru1(1)
{
	// boot up the PRUSS by probing the remoteproc driver
	this->sock.sendcmd("DISABLE_0");
	this->sock.sendcmd("DISABLE_1");

	this->bootUp();
	
}

/**
 * Destructor for PRUSS
 *
 * Shuts Down the PRU-ICSS at the end of the program
 */
PRUSS::~PRUSS()
{
	if(this->isOn())
		this->shutDown();
}

/**
 * Check if the PRU-ICSS has booted up
 *
 * @return boolean value true if the pru_rproc driver has been probed
 */
bool PRUSS::isOn()
{
	return this->on;
}

/**
 * Boot Up the PRU-ICSS
 *
 * Probe the pru_rproc remoteproc driver so that all the sysfs entries for controlling the PRUs are created.
 * Sets the 'state' of the PRU to STOPPED after successfully probing the driver
 *
 * @return 0 if pru_rproc is probed successfully
 */
int PRUSS::bootUp()
{
	if(this->on)
		return -EALREADY;
	int ret = stoi(this->sock.sendcmd("PROBE_RPROC")); //send command
	if(!ret) {
		this->on = true;
		this->pru0.state = this->pru1.state = STOPPED; //PRU Cores are disabled after bootup
	}
	return ret; 
}

/**
 * Shut Down the PRU-ICSS
 *
 * Stops both the PRUs and unprobes the pru_rproc remoteproc driver from the kernel
 * Sets the 'state' of both PRUs to NONE after successfully doing so
 *
 * @return 0 if pru_rproc is unprobed successfully
 */
int PRUSS::shutDown()
{
	if(!this->on)
		return -EALREADY;
	this->pru0.disable();
	this->pru1.disable();
	int ret = stoi(this->sock.sendcmd("UNPROBE_RPROC"));
	if(!ret) {
		this->on = false;
		this->pru0.state = this->pru1.state = NONE;
	}
	return ret;
}

/**
 * Restart the PRU-ICSS system
 *
 * Shuts down and restarts the PRU-ICSS 
 *
 * @see shutDown()
 * @see bootUp()
 */
void PRUSS::restart()
{
	this->shutDown();
	this->bootUp();
}

/**
 * Constructor for PRU
 *
 * Sets the PRU number: 0 or 1 according to which PRU the object represents.
 * Sets the RPMsg communication channel name for that PRU
 * Restarts the PRU after doing so.
 *
 * @param number represents the PRU number (0 or 1)
 */
PRU::PRU(int number)
{
	this->number = number;
	this->setChannel(); //set default channels 
	this->reset();
}

/**
 * Constructor for PRU
 *
 * Sets the PRU number: 0 or 1 according to which PRU the object represents.
 * Sets the RPMsg communication channel name for that PRU
 * This constructor overloads PRU(int number) such that it also loads the 
 * firmware into the PRU
 *
 * @param number represents the PRU number (0 or 1)
 * @param fw is the PRU relative/absolute path for the firmware that needs to be loaded
 */
PRU::PRU(int number, string fw)
{
	this->number = number;
	this->setChannel(); //set default channels
	this->load(fw);
}

/**
 * Enable the PRU
 *
 * Starts the PRU by echoing start into the /sys/class/remoteproc/remoteprocN/state directory
 *
 * @return 0 if write was done successfully
 */
int PRU::enable()
{
	if(this->state == NONE)
		return -ENODEV;
	if(this->state == RUNNING || this->state == HALTED)
		return -EALREADY;
	int ret = stoi(this->sock.sendcmd("ENABLE_"+to_string(this->number)));
	if(!ret)
		this->state = RUNNING;
	return ret;
}

/**
 * Disable the PRU
 *
 * Stops the PRU by echoing stop into the /sys/class/remoteproc/remoteprocN/state directory
 *
 * @return 0 if the write was done successfully
 */
int PRU::disable()
{
	if(this->state == NONE)
		return -ENODEV;
	if(this->state == STOPPED)
		return -EALREADY;
	int ret = stoi(this->sock.sendcmd("DISABLE_"+to_string(this->number)));
	if(!ret)
		this->state = STOPPED;
	return ret;
}

/**
 * Reset the PRU
 *
 * Completely stops the PRU execution and starts again from first instruction.
 * @see PRU::disable()
 * @see PRU::enable()
 * @return 0 if writes were done successfully
 */
int PRU::reset()
{
	this->disable(); 
	return this->enable(); 
}

/**
 * Pause the PRU execution
 * 
 * This function pauses the PRU execution in such a way that the execution can be resumed from the same point where it stopped. (This is not the case when echoing `stop` into /sys/class/remoteproc/remoteprocN because then the PRU would have to re start from its first instruction)
 * 
 * The PRU is paused by echoing '1' into /sys/kernel/debug/remoteproc/remoteprocN/single_step
 *
 * @return 0 if the echoing is done successfully 
 */
int PRU::pause()
{
	if(this->state == NONE)
		return -ENODEV;
	if(this->state == HALTED)
		return -EALREADY;
	int ret = stoi(this->sock.sendcmd("PAUSE_"+to_string(this->number)));
	if(!ret)
		this->state = HALTED;
	return ret;
}

/**
 * Resume PRU execution in single step mode
 *
 * This function is used to execute the PRU in single_step mode.
 * Every time this function is called, the PRU executes one instruction cycle by
 * echoing '1' into /sys/kernel/debug/remoteproc/remoteprocN/single_step
 *
 * @return 0 if the echoing is done successfully 
 */
int PRU::resume()
{
	if(this->state == NONE || this->state == STOPPED)
		return -ENODEV;
	if(this->state == RUNNING)
		return -EALREADY;
	int ret = stoi(this->sock.sendcmd("RESUME_"+to_string(this->number)));
	if(!ret)
		this->state = RUNNING;
	return ret;
}

/**
 * Show PRU register values
 *
 * Displays all the current values of the PRU Control Registers and
 * displats all the  PRU General Purpose Registers if the PRU is not running.
 *
 * @return The entire contents of the regs file in /sys/kernel/debug/remoteproc/remoteprocN The return string is large more than 1kB
 */
string PRU::showRegs()
{
	return this->sock.sendcmd("GETREGS_" + to_string(this->number));
}

/**
 * Install compiled firmware to the PRU.
 *
 * Installs compiled PRU firmware by copying it into /lib/firmware and echoing the name of the elf file into /sys/class/remoteproc/remoteprocN/firmware
 *
 * @param fw is the relative path/absolute path to the firmware that is to be loaded to the PRU.
 * @return 0 if the firmware is loaded successfully.
 */
int PRU::load(string fw)
{
	this->disable();
	char buf[PATH_MAX];
	realpath(fw.c_str(), buf);
	std::string fullPath(buf);
	int ret = stoi(this->sock.sendcmd("LOAD_" + to_string(this->number) + " " + fullPath));
	this->enable();
	return ret;
}

/**
 * Set the RPMsg communication channel according to the "pruss_api"
 * driver. This driver creates "pruss_api_pru0" channel for PRU0
 * and "pruss_api_pru1" for PRU1.
 *
 * chanPort maps to which PRU object is being used.
 * chanName specifies the channel name that is common for both the PRUs.
 */
void PRU::setChannel()
{
	this->chanPort = (this->number)?1:0;
	this->chanName = "pruss_api_pru";
}

/**
 * Set the RPMsg communication channel of the PRU object.
 *
 * This function is to be used to a RPMsg channel of a different name than 
 * pruss_api_pruN.
 * For ex, if the firmware uses rpmsg_pru31
 *
 * @param port specifies which port is to be used.
 * @param name specifies the name of the rpmsg channel to be used.
 * @return 0
 */
int PRU::setChannel(int port, string name)
{
	if(port < 0)
		return -EINVAL;
	this->chanPort = port;
	this->chanName = name;
	return 0;
}

/**
 * Get the current state of the PRU object
 *
 * Used to know the current whether the PRU is currently in a 
 * stopped, running, halted or none state.
 *
 * @see enum State
 * @return state of the PRU
 */
State PRU::getState()
{
	return this->state;
}

/**
 * Send a message to PRU in the form of characters.
 *
 * Send a string of characters through pruss_api_pruN to the PRUN
 * of maximum length of 496 bytes (512 limit - 16 header size) 
 * The firmware that uses RPMsg MUST be running so that the communication channel is created.
 *
 * @param message of not more than 496 characters.
 * @return 0 if the message is written to the RPMsg channel successfully else appropriate error code.
 */
int PRU::sendMsg_string(string message)
{
        string cmd = "SENDMSG s "+this->chanName+" "+to_string(this->chanPort)+" "+message;
        return stoi(this->sock.sendcmd(cmd)); // eg. sendmsg s rpmsg_pru 31 hi!
}

/**
 * Send a raw integer value to PRU.
 *
 * It is difficult to reconstruct data on the PRU side if data is sent as a string. This is because
 * the PRU compilers have limited memory and can't perform string operations.
 *
 * Sending data in raw form makes it easier for it to be reconstructed on the PRU side.
 *
 * @param message string representing 1-byte integer that needs to be passed to the PRU. The string will be converted to integer once passed to the daemon.
 * @return 0 if the message is written to the RPMsg channel successfully else appropriate error code.
 */
void PRU::sendMsg_raw(string message)
{
        string cmd = "SENDMSG r "+this->chanName+" "+to_string(this->chanPort)+" "+message;
        this->sock.sendcmd(cmd); // eg. sendmsg r rpmsg_pru 31 1281
}

/**
 * Receive message from PRU 
 *
 * Receive a string message from pruss_api_pruN channel.
 * The message should already be present in the buffer before this function
 * is called.
 *
 * @return the message received from the PRU at the RPMsg channel as a string, if 
 * no message is found, return '\n'
 */
string PRU::getMsg()
{
	return this->sock.sendcmd("GETMSG "+this->chanName+" "+to_string(this->chanPort));
}


/**
 * Wait till an event occurs at the RPMsg channel of the given PRU
 *
 * Waits indefinitely on the RPMsg channel pruss_api_pruN in /dev until an event occurs.
 *
 * @return 0 if event occurs 
 */
int PRU::waitForEvent()
{
	return stoi(this->sock.sendcmd("EVENTWAIT "+this->chanName+" "+to_string(this->chanPort)));
}

/**
 * Wait till an event occurs at the RPMsg channel of the given PRU
 *
 * Waits on the RPMsg channel pruss_api_pruN in /dev until an event occurs 
 * within the specified time limit.
 *
 * @param time integer that specifies for how much time to wait for any event before returning.
 * @return 0 if event occurs or appropriate error code.
 */
int PRU::waitForEvent(int time)
{
	return stoi(this->sock.sendcmd("EVENTWAIT "+this->chanName+" "+to_string(this->chanPort)+" "+to_string(time)));
}


/**
 * Read from PRU SRAM/DRAM0/DRAM1
 *
 * Read a 1-byte integer value from the PRU memory block specified in the parameter. The 1-byte integer value will be read
 * as a string.
 *
 * @param mem describes which PRU memory block is to be used from the enumeration Memory.
 * @param offset sets the address offset for the memory block.
 * @see enum Memory
 * @return the 1-byte integer value present at thae address offset as a string.
 */
string PRU::mem_read(Memory mem, string offset){
    if (mem == SHARED)
        return this->sock.sendcmd("MEMREAD_S "+offset);
    else if (mem == DATA0 || mem == DATA1)
        return this->sock.sendcmd("MEMREAD_D"+to_string(mem)+" "+offset);
    else 
        return to_string(-EINVAL);
}

/**
 * Write to PRU SRAM/DRAM0/DRAM1
 *
 * Write a 1-byte integer value to the PRU memory block specified in the parameter. The 1-byte integer value must be specified
 * as a string.
 *
 * @param mem describes which PRU memory block is to be used from the enumeration Memory.
 * @param offset sets the address offset for the memory block.
 * @param data the integer data that needs to be written to that address offset.
 * @see enum Memory
 * @return "0" if write is done else the negative of EINVAL
 */
string PRU::mem_write(Memory mem, string offset, string data){
    if (mem == SHARED)
        return this->sock.sendcmd("MEMWRITE_S "+offset+" "+data);
    else if (mem == DATA0 || mem == DATA1)
        return this->sock.sendcmd("MEMWRITE_D"+to_string(mem)+" "+offset+" "+data);
    else 
        return to_string(-EINVAL);
}
