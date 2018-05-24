#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/poll.h>

void modprobe()
{
	char cmd[] = "modprobe pru_rproc";
	if(system(cmd) == -1)
	{
		fprintf(stderr, "Error probing Remoteproc driver");
		exit(1);
	}
}

void modunprobe()
{
	char cmd[] = "modprobe -r pru_rproc";
	if(system(cmd) == -1)
	{
		fprintf(stderr, "Error unprobing Remoteproc driver");
		exit(1);
	}
}

void pru_enable(int pru_no)
{
	if(pru_no != 0 && pru_no != 1)
	{
		fprintf(stderr, "PRU no can be 0 or 1 only");
		exit(1);
	}
	
	char path[40];
	sprintf(path, "/sys/class/remoteproc/remoteproc%d/state", pru_no+1);
	FILE *fp;
	if((fp = fopen(path, "w")) == NULL)
	{
		fprintf(stderr, "Error enabling the PRUs");
		exit(1);
	}
	fprintf(fp, "start");
	fclose(fp);

}	

void pru_disable(int pru_no)
{
	if(pru_no != 0 && pru_no != 1)
	{
		fprintf(stderr, "PRU no can be 0 or 1 only");
		exit(1);
	}
	
	char path[40];
	sprintf(path, "/sys/class/remoteproc/remoteproc%d/state", pru_no+1);
	FILE *fp;
	if((fp = fopen(path, "w")) == NULL)
	{
		fprintf(stderr, "Error disabling the PRUs");
		exit(1);
	}
	fprintf(fp, "stop");
	fclose(fp);

}	

void pru_reset(int pru_no)
{
	pru_disable(pru_no);
	pru_enable(pru_no);

}

void exec_program(char* filepath, int pru_no)
{
	pru_disable(pru_no);
	char cmd[100];
	sprintf(cmd, "cp %s /lib/firmware/am335x-pru%d-fw", filepath, pru_no);
	if(system(cmd) == -1)
	{
		fprintf(stderr, "Error loading firmware");
		exit(1);
	}
	pru_enable(pru_no);
}

void send_msg(char* message, char* channel)
{
	char devfile[30];
	sprintf(devfile, "/dev/%s", channel);
	FILE *fp;
	if((fp = fopen(devfile, "w")) == NULL)
	{
		fprintf(stderr, "Channel does not exist");
		exit(1);
	}
	fprintf(fp, "%s\n", message);
	fclose(fp);
}

void get_msg(char* channel, char* message)
{
	char devfile[30];
	sprintf(devfile, "/dev/%s", channel);
	FILE *fp;
	if((fp = fopen(devfile, "r")) == NULL)
	{
		fprintf(stderr, "Channel does not exist");
		exit(1);
	}

	fscanf(fp, "%s", &message);
	fclose(fp);
}













