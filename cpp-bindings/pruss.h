#ifndef PRUSS_H_
#define PRUSS_H_

#include <stdlib.h>
#include <linux/limits.h> // realpath
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

//enumeration which describes which memory access is required
enum Memory
{
    DATA0 = 0,
    DATA1 = 1,
    SHARED = 3
};

/** 
 * class Socket
 */
class Socket
{
	private:
		const char* socketpath;
		struct sockaddr_un addr;
		int fd;
		Socket();
		bool conn();
		bool disconn();
		std::string sendcmd(std::string);
		friend class PRUSS; //Only these classes have access to the Socket class
		friend class PRU; //Only these classes have access to the Socket class
};

/** 
 * class PRU
 */
class PRU
{
	private:
		int number;
		int chanPort;
		std::string chanName;
		Socket sock;
		State state = NONE;	
		PRU(int);
		PRU(int, std::string);
		friend class PRUSS; //Only PRUSS class can call the PRU class constructors
	public:
		int enable();
		int disable();
		int reset();
		int pause();
		int resume();
		std::string showRegs();
		int load(std::string);
		void setChannel();
		int setChannel(int, std::string);
		State getState();
                int sendMsg_string(std::string);
                void sendMsg_raw(std::string);
		std::string getMsg();
		int waitForEvent();
		int waitForEvent(int);
                std::string mem_read(Memory mem, std::string);
                std::string mem_write(Memory mem, std::string, std::string);
};

/** 
 * class PRUSS
 */
class PRUSS
{
	private:
		bool on = false;
		Socket sock;
		PRUSS();
		~PRUSS();
	public:
		static PRUSS& get();
		PRU pru0;
		PRU pru1;
		bool isOn();
		int bootUp();
		int shutDown();
		void restart();

};
#endif
