#ifndef PRUSS_H_
#define PRUSS_H_

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define PRU_OFFLINE 0
#define PRU_RUNNING 1
#define PRU_HALTED 2

#define SYSFS_PATH "/sys/class/remoteproc/remoteproc"
#define FW_PATH "/lib/firmware/am335x-pru"
#define RPMSG_PATH "/dev/rpmsg_pru"

#define PRU_ICSS 0x4A300000
#define PRU_ICSS_LEN 512*1024
#define PRU_DRAM0 0x000
#define PRU_DRAM1 0x2000
#define PRU_SRAM 0x10000

class PRU
{
	private:
		int number;
		int state;
		std::string channel;
		std::string sysfs;

	public:
		PRU(int);
		PRU(int, std::string);
		static void modprobe();
		static void modunprobe();
		void enable();
		void disable();
		void reset();
		void load(std::string);
		void set_channel(int);
		std::string get_state();
		void send_msg(std::string);
		std::string get_msg();
		void wait_for_event();
		void mem_writeint(int, unsigned int);
		unsigned int mem_readint(int);
};

#endif
