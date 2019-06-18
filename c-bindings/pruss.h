#include <stdio.h>
#include <string.h>
#include <sys/socket.h> 
#include <sys/un.h> 
#include <unistd.h> 
#include <errno.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <limits.h>

bool sock_connect();
char* sendcmd(char* cmd);
bool sock_disconnect();

enum State{
    NONE,
    STOPPED,
    RUNNING,
    HALTED
};

struct Socket{
    int fd;
    const char* socketpath;
    struct sockaddr_un addr;
};

struct PRU{
    int number;
    int chanPort;
    char *chanName;
    Socket sock;
    State state = NONE;
};

struct PRUSS{
    bool on = false;
    Socket sock;
    PRU pru0;
    PRU pru1;
};

void test(PRU*);
void PRUSS_init(PRUSS *pruss);
int PRUSS_bootUp(PRUSS *pruss);
void PRUSS_dest(PRUSS *pruss);
int PRUSS_shutDown(PRUSS *pruss);
void PRUSS_restart(PRUSS *pruss);

void PRU_init(PRU* pru, int number);
void PRU_init(PRU* pru, int number, char* fw);
int PRU_enable(PRU* pru);
int PRU_disable(PRU* pru);
int PRU_reset(PRU* pru);
int PRU_pause(PRU *pru);
int PRU_resume(PRU* pru);
char* PRU_showRegs(PRU* pru);
int PRU_load(PRU* pru, char* fw);
void PRU_setChannel(PRU* pru);
int PRU_setChannel(PRU* pru, int port, char* name);
State PRU_getState(PRU* pru);
int PRU_sendMsg(PRU* pru, char *message);
char* PRU_getMsg(PRU* pru);
