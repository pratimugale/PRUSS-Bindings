#ifndef PRUSS_H_
#define PRUSS_H_

#include <stdlib.h>
#include <string> // string handling
#include <sys/socket.h> // socket
#include <sys/un.h> // socket
#include <unistd.h> // close()
#include <errno.h> // error codes

//enumeration which describes the states of a PRU Core 
enum State
{
	NONE,
	STOPPED,
	RUNNING,
	HALTED
};

//socket class
class Socket
{
	private:
		const char* socketpath;
		struct sockaddr_un addr;
		int fd;
	
	public:
		Socket();
		bool conn();
		bool disconn();
		std::string sendcmd(std::string);
};

class PRU
{
	private:
		int number;
		int channel;
		Socket sock;
	public:
		State state = NONE;	
		PRU(int);
		PRU(int, std::string);
		int enable();
		int disable();
		int reset();
		int pause();
		int resume();
		std::string showRegs();
		int load(std::string);
		void setChannel();
		int setChannel(int);
		State getState();
		int sendMsg(std::string);
		std::string getMsg();
		int waitForEvent();
		int waitForEvent(int);
};

class PRUSS
{
	private:
		bool on = false;
		Socket sock;
	public:
		PRU pru0;
		PRU pru1;
		PRUSS();
		bool isOn();
		int bootUp();
		int shutDown();
		void restart();

};

#endif
