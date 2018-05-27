#include "pruss.h"

using namespace std;

PRU::PRU(int number)
{
	if(number != 0 &&  number != 1)
	{
		cerr << "Error Initialising the PRU: The PRU Core number can be 0 or 1 only\n";
		exit (EXIT_FAILURE);
	}
	this->number = number;
	this->sysfs = SYSFS_PATH + to_string(number + 1) + "/state";
	this->set_channel(30 + number);

	PRU::modprobe();
	this->reset();
}

PRU::PRU(int number, string fw)
{
	if(number != 0 && number != 1)
	{
		cerr << "Error Initialising the PRU: The PRU Core number can be 0 or 1 only\n";
		exit(EXIT_FAILURE);
	}
	this->number = number;
	this->set_channel(30 + number);
	this->sysfs = SYSFS_PATH + to_string(number + 1) + "/state";

	PRU::modprobe();
	this->load(fw);
}

void PRU::modprobe()
{
	if(system("/sbin/modprobe pru_rproc"))
	{
		cerr << "Error probing the remoteproc driver\n";
		exit(EXIT_FAILURE);
	}
}

void PRU::modunprobe()
{
	if(system("/sbin/modprobe -r pru_rproc"))
	{
		cerr << "Error removing the remoteproc driver\n";
		exit(EXIT_FAILURE);
	}
}

string PRU::get_state()
{
	string status;
	ifstream statefile;
	statefile.open (this->sysfs);
	if(!(statefile.is_open()))
	{
		cerr << "Error fetching state\n";
		exit(EXIT_FAILURE);
	}
	statefile >> status;
	statefile.close();
	return status;
}

void PRU::enable()
{
	ofstream statefile;
	statefile.open (this->sysfs);
	if(this->get_state() == "offline")
	{
		if(!(statefile.is_open()))
		{
			cerr << "Error Enabling the PRU\n";
			exit(EXIT_FAILURE);
		}
		statefile << "start";
		statefile.close();
		this->state = PRU_RUNNING;
	}
}

void PRU::disable()
{
	ofstream statefile;
	statefile.open (SYSFS_PATH + std::to_string(this->number + 1) + "/state");
	if(this->get_state() == "running")
	{
		if(!(statefile.is_open()))
		{
			cerr << "Error Disabling the PRU\n";
			exit(EXIT_FAILURE);
		}
		statefile << "stop";
		statefile.close();
		this->state = PRU_OFFLINE;
	}
}

void PRU::reset()
{
	this->disable();
	this->enable();
}

void PRU::load(string fw)
{
	this->disable();
	if(system(("cp "+ fw + " " + FW_PATH + to_string(this->number) + "-fw").c_str()))
	{
		cerr << "Error loading firmware\n";
		exit(EXIT_FAILURE);
	}
	this->enable();
}

void PRU::set_channel(int ch)
{
	this->channel = RPMSG_PATH + to_string(ch);
}

void PRU::send_msg(string msg)
{
	ofstream devfile ((this->channel).c_str());
	if(!devfile)
	{
		cerr << "Error Opening Channel " << this->channel << '\n';
		exit(EXIT_FAILURE);
	}
	devfile << msg;
	devfile.close();
}

string PRU::get_msg()
{
	string msg;
	ifstream devfile (this->channel);
	if(!devfile)
	{
		std::cerr << "Error Opening Channel " << this->channel << '\n';
		exit(EXIT_FAILURE);
	}
	devfile >> msg;
	devfile.close();
	return msg;
}

void PRU::wait_for_event()
{
	ifstream devfile (this->channel);
	if(!devfile)
	{
		std::cerr << "Error Opening " << this->channel << '\n';
		exit(EXIT_FAILURE);
	}
	char tmp;
	devfile >> tmp;
	devfile.close();
}

void PRU::mem_writeint(int offset, unsigned int data)
{
	unsigned int * pru_addr = NULL;
	int fd = open("/dev/mem", O_RDWR);
	pru_addr = (unsigned int*)mmap(NULL, PRU_ICSS_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PRU_ICSS);
	if(pru_addr == MAP_FAILED)
	{
		cerr << "Failed to map PRU memory" << '\n';
		exit(EXIT_FAILURE);
	}
	pru_addr[(this->number)?PRU_DRAM1:PRU_DRAM0] = data;
	close(fd);
}

unsigned int PRU::mem_readint(int offset)
{
	unsigned int data;
	unsigned int * pru_addr = NULL;
	int fd = open("/dev/mem", O_RDWR);
	pru_addr = (unsigned int*)mmap(NULL, PRU_ICSS_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PRU_ICSS);
	if(pru_addr == MAP_FAILED)
	{
		cerr << "Failed to map PRU memory" << '\n';
		exit(EXIT_FAILURE);
	}
	data = pru_addr[(this->number)?PRU_DRAM1:PRU_DRAM0];

	close(fd);
	return data;
}
