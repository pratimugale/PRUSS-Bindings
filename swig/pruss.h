#ifndef PRUSS_H_
#define PRUSS_H_

#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

//enumeration which describes the states of a PRU Core 
enum State
{
	NONE,
	STOPPED,
	RUNNING,
	HALTED
};

class PRUSS
{
	private:
		bool on;
	
	public:
		PRU pru0;
		PRU pru1;
		PRUSS();
		bool isOn();
		int bootUp();
		int shutDown();

};

class PRU
{
	private:
		State state;
		int number;
		std::string channel;
		PRU(int);
		PRU(int, std::string);

	public:
		int enable();
		int disable();
		int reset();
		int pause();
		int resume();
		std::string showRegs();
		int load(std::string);
		int setChannel(int);
		State getState();
		int sendMsg(std::string);
		std::string getMsg();
		int waitForEvent();
};

//socket functions

//connect to the socket and return the socket fd 
int socket_init();

//send the command to the socket server and return the received message
std::string socket_send(int, std::string);

//close the socket connection.
void socket_close(int);

#endif
