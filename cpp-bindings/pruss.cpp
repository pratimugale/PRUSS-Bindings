#include "pruss.h"
#include <iostream>

using namespace std;

Socket::Socket()
{
	this->fd = -1;
	this->socketpath = "/tmp/prussd.sock";
}

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

bool Socket::disconn()
{
	if(this->fd == -1)
		return false;
	close(this->fd);
	this->fd = -1;
	return true;
}

PRUSS& PRUSS::get()
{
	static PRUSS p;
	return p;
}

PRUSS::PRUSS() : pru0(0), pru1(1)
{
	// boot up the PRUSS by probing the remoteproc driver
	this->sock.sendcmd("DISABLE_0");
	this->sock.sendcmd("DISABLE_1");

	this->bootUp();
	
}

PRUSS::~PRUSS()
{
	if(this->isOn())
		this->shutDown();
}

bool PRUSS::isOn()
{
	return this->on;
}

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

void PRUSS::restart()
{
	this->shutDown();
	this->bootUp();
}

PRU::PRU(int number)
{
	this->number = number;
	this->setChannel(); //set default channels 
	this->reset();
}

PRU::PRU(int number, string fw)
{
	this->number = number;
	this->setChannel(); //set default channels
	this->load(fw);
}

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

int PRU::reset()
{
	this->disable(); 
	return this->enable(); 
}

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

string PRU::showRegs()
{
	return this->sock.sendcmd("GETREGS_" + to_string(this->number));
}

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

void PRU::setChannel()
{
	this->chanPort = (this->number)?1:0;
	this->chanName = "pruss_api_pru";
}

int PRU::setChannel(int port, string name)
{
	if(port < 0)
		return -EINVAL;
	this->chanPort = port;
	this->chanName = name;
	return 0;
}

State PRU::getState()
{
	return this->state;
}

int PRU::sendMsg_string(string message)
{
        string cmd = "SENDMSG s "+this->chanName+" "+to_string(this->chanPort)+" "+message;
        return stoi(this->sock.sendcmd(cmd)); // eg. sendmsg s rpmsg_pru 31 hi!
}

void PRU::sendMsg_raw(string message)
{
        string cmd = "SENDMSG r "+this->chanName+" "+to_string(this->chanPort)+" "+message;
        this->sock.sendcmd(cmd); // eg. sendmsg r rpmsg_pru 31 1281
}

string PRU::getMsg()
{
	return this->sock.sendcmd("GETMSG "+this->chanName+" "+to_string(this->chanPort));
}

int PRU::waitForEvent()
{
	return stoi(this->sock.sendcmd("EVENTWAIT "+this->chanName+" "+to_string(this->chanPort)));
}

int PRU::waitForEvent(int time)
{
	return stoi(this->sock.sendcmd("EVENTWAIT "+this->chanName+" "+to_string(this->chanPort)+" "+to_string(time)));
}


string PRU::mem_read(Memory mem, string offset){
    if (mem == SHARED)
        return this->sock.sendcmd("MEMREAD_S "+offset);
    else if (mem == DATA0 || mem == DATA1)
        return this->sock.sendcmd("MEMREAD_D"+to_string(mem)+" "+offset);
    else 
        return to_string(-EINVAL);
}

string PRU::mem_write(Memory mem, string offset, string data){
    if (mem == SHARED)
        return this->sock.sendcmd("MEMWRITE_S "+offset+" "+data);
    else if (mem == DATA0 || mem == DATA1)
        return this->sock.sendcmd("MEMWRITE_D"+to_string(mem)+" "+offset+" "+data);
    else 
        return to_string(-EINVAL);
}
