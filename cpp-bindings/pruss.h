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
class Sock
{
	private:
		const char* socketpath;
		struct sockaddr_un addr;
	
	public:
		int fd;
		Sock();
		bool conn();
		std::string sendcmd(std::string);
		void disconn();
};

class PRU
{
	private:
		int number;
		int channel;
		Sock sock;
		
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
		Sock sock;
	
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
