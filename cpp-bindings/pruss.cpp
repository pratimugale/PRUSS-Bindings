#include "pruss.h"

using namespace std;

Sock::Sock()
{
	this->fd = -1;
	this->socketpath = "/tmp/prusocket";
}

bool Sock::conn()
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

string Sock::sendcmd(string command)
{
	if(!this->conn()) // Connect to the socket 
		return to_string(ECONNREFUSED);
	
	int nbytes;
	string received;
	char buf[1024], rec[1024]; //buffers to store command and reply
	nbytes = snprintf(buf, sizeof(buf), command.c_str()); //store command in buf
	buf[nbytes] = '\n';
	send(this->fd, buf, strlen(buf), 0); // send command over the socket connection

	nbytes = recv(this->fd, rec, sizeof(rec), 0); // receive reply from server
	rec[nbytes] = '\0'; // string boundary
	received = std::string(rec); // convert char* to string
	
	this->disconn(); // disconnect from the socket connection
	
	return received;
}

void Sock::disconn()
{
	close(this->fd);
	this->fd = -1;
}

PRUSS::PRUSS() : pru0(0), pru1(1)
{
	// boot up the PRUSS by probing the remoteproc driver
	this->bootUp();
	
}

bool PRUSS::isOn()
{
	return this->on;
}

int PRUSS::bootUp()
{
	if(this->on)
		return EALREADY;
	int ret = stoi(this->sock.sendcmd("probe_rproc")); //send command
	if(!ret) {
		this->on = true;
		this->pru0.state = this->pru1.state = STOPPED; //PRU Cores are disabled after bootup
	}
	return ret; 
}

int PRUSS::shutDown()
{
	if(!this->on)
		return EALREADY;
	int ret = stoi(this->sock.sendcmd("unprobe_rproc"));
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
		return ENODEV;
	if(this->state == RUNNING || this->state == HALTED)
		return EALREADY;
	int ret = stoi(this->sock.sendcmd("enable"+to_string(this->number)));
	if(!ret)
		this->state = RUNNING;
	return ret;
}

int PRU::disable()
{
	if(this->state == NONE)
		return ENODEV;
	if(this->state == STOPPED)
		return EALREADY;
	int ret = stoi(this->sock.sendcmd("disable"+to_string(this->number)));
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
		return ENODEV;
	if(this->state == HALTED)
		return EALREADY;
	int ret = stoi(this->sock.sendcmd("pause"+to_string(this->number)));
	if(!ret)
		this->state = HALTED;
	return ret;
}

int PRU::resume()
{
	if(this->state == NONE || this->state == STOPPED)
		return ENODEV;
	if(this->state == RUNNING)
		return EALREADY;
	int ret = stoi(this->sock.sendcmd("resume"+to_string(this->number)));
	if(!ret)
		this->state = RUNNING;
	return ret;
}

string PRU::showRegs()
{
	return this->sock.sendcmd("showregs" + to_string(this->number));
}

int PRU::load(string fw)
{
	this->disable();
	if(system(("cp "+ fw + " /tmp/pru" + to_string(this->number)).c_str())) {
		return ENOENT;	
	}
	int ret = stoi(this->sock.sendcmd("load" + to_string(this->number)));
	this->enable();
	return ret;
}

void PRU::setChannel()
{
	this->channel = (this->number)?31:30;	
}

int PRU::setChannel(int channel)
{
	if(channel < 0)
		return EINVAL;
	this->channel = channel;
	return 0;
}

State PRU::getState()
{
	return this->state;
}

int PRU::sendMsg(string message)
{
	return stoi(this->sock.sendcmd("sendmsg " + to_string(this->channel) + " " + message)); // eg. sendmsg 31 hi!
}

string PRU::getMsg()
{
	return this->sock.sendcmd("getmsg " + to_string(this->channel));
}

int PRU::waitForEvent()
{
	return stoi(this->sock.sendcmd("eventwait " + to_string(this->channel)));
}

int PRU::waitForEvent(int timeout)
{
	return stoi(this->sock.sendcmd("eventwait " + to_string(this->channel) + " " + to_string(timeout)));
}


