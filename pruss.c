#include "pruss.h"

char err[4];
char rec[1024];
char command[100];

Socket sock;

/*int main(void){
     
    bool x = sock_connect();
    printf("%i\n",sock.fd);
    printf("%s\n",sock.socketpath);
    printf("%d\n", x);
    
    char commanda[] = "ENABLE_0";
    char *ret = sendcmd(commanda);
    printf("%s\n", ret);
    PRU p0 = pruss.pru0;
    PRU p1 = pruss.pru1;
    void change();
    return 0;
}*/

bool sock_connect(){
    sock.fd = -1;
    sock.socketpath = "/tmp/prussd.sock";   
    sock.fd = socket(AF_UNIX, SOCK_STREAM, 0);
    memset(&sock.addr, 0, sizeof(sock.addr));
    sock.addr.sun_family = AF_UNIX;
    strncpy(sock.addr.sun_path, sock.socketpath, sizeof(sock.addr.sun_path)-1);
    if(connect(sock.fd, (struct sockaddr*)&sock.addr, sizeof(sock.addr)) == -1)
        return false;

    return true;
}

char* sendcmd(char *cmd){
    if(!sock_connect()){ 
        sprintf(err, "%d", ECONNREFUSED);
        return err;
    }

    int nbytes;
    char buf[1024]; 
    nbytes = snprintf(buf, sizeof(buf), cmd); 
    buf[nbytes] = '\n';
   // printf("Sent: %s\n", buf);
    send(sock.fd, buf, strlen(buf), 0); 

    nbytes = recv(sock.fd, rec, sizeof(rec), 0); 
    rec[nbytes] = '\0'; 	
   // printf("Received: %s\n", rec);

    sock_disconnect(); 
	
    return rec;

}

bool sock_disconnect(){
    if(sock.fd == -1)
        return false;
    close(sock.fd);
    sock.fd = -1;
    return true;
}

void PRUSS_init(PRUSS* pruss){
    // This is the constructor for PRUSS in cpp
    sendcmd("DISABLE_0");
    sendcmd("DISABLE_1");
    PRUSS_bootUp(pruss);
}

int PRUSS_bootUp(PRUSS* pruss){
    if(pruss->on)
        return -EALREADY;
    int ret = atoi(sendcmd("PROBE_RPROC"));
    //printf("DEBUG rprocprobe%i\n", ret);
    if(!ret){
        pruss->on = true;
        pruss->pru0.state = pruss->pru1.state = STOPPED;
    }
    return ret;
}

void PRUSS_dest(PRUSS* pruss){
    if(pruss->on)
        PRUSS_shutDown(pruss);
}

int PRUSS_shutDown(PRUSS* pruss){
    if(!pruss->on)
        return -EALREADY;
    PRU_disable(&pruss->pru0);
    PRU_disable(&pruss->pru1);
    int ret = atoi(sendcmd("UNPROBE_RPROC"));
    if(!ret){
        pruss->on = false;
        pruss->pru0.state = pruss->pru1.state = NONE;
    }
    return ret;
}

void PRUSS_restart(PRUSS* pruss){
    PRUSS_shutDown(pruss);
    PRUSS_bootUp(pruss);
}

void PRU_init(PRU* pru, int number){
    pru->number = number; 
    PRU_setChannel(pru);
    PRU_reset(pru);
}

void PRU_init(PRU* pru, int number, char* fw){
    pru->number = number; 
    PRU_setChannel(pru);
    PRU_load(pru, fw);
}

int PRU_enable(PRU* pru){
    if(pru->state == NONE)
        return -ENODEV;
    if(pru->state == RUNNING || pru->state == HALTED)
        return -EALREADY;
    char tmp[2];
    sprintf(tmp, "%d", pru->number);
    strcpy(command, "ENABLE_");
    strcat(command, tmp);
    int ret = atoi(sendcmd(command));
    if(!ret)
        pru->state = RUNNING;
    return ret;
}

int PRU_disable(PRU* pru){
    if(pru->state == NONE)
        return -ENODEV;
    if(pru->state == STOPPED)
        return -EALREADY;
    char tmp[2];
    sprintf(tmp, "%d", pru->number);
    strcpy(command, "DISABLE_");
    strcat(command, tmp);
    int ret = atoi(sendcmd(command));
    if(!ret)
        pru->state = STOPPED;
    return ret;
}

int PRU_reset(PRU* pru){
    PRU_disable(pru);
    return PRU_enable(pru);
}

int PRU_pause(PRU *pru){
    if (pru->state == NONE)
        return -ENODEV;
    if (pru->state == HALTED)
        return -EALREADY;
    char tmp[2];
    sprintf(tmp, "%d", pru->number);
    strcpy(command, "PAUSE_");
    strcat(command, tmp);
    int ret = atoi(sendcmd(command));
    if(!ret)
        pru->state = HALTED;
    return ret;
}

int PRU_resume(PRU* pru){
    if(pru->state == NONE || pru->state == STOPPED)
        return -ENODEV;
    if(pru->state == RUNNING)
        return -EALREADY;
    char tmp[2];
    sprintf(tmp, "%d", pru->number);
    strcpy(command, "RESUME_");
    strcat(command, tmp);
    int ret = atoi(sendcmd(command));
    if(!ret)
        pru->state = RUNNING;
    return ret;
}

char* PRU_showRegs(PRU* pru){
    char tmp[2];
    sprintf(tmp, "%d", pru->number);
    strcpy(command, "GETREGS_");
    strcat(command, tmp);
    return sendcmd(command);
}

int PRU_load(PRU* pru, char* fw){
    PRU_disable(pru);
    char buf[PATH_MAX];
    char *res = realpath(fw, buf); 
    char tmp[2];
    sprintf(tmp, "%d", pru->number);
    strcpy(command, "LOAD_");
    strcat(command, tmp);
    char space[] = " ";
    strcat(command, space);
    strcat(command, buf);
    int ret = atoi(sendcmd(command));
    PRU_enable(pru);
    return ret; 
}

void PRU_setChannel(PRU* pru){
    pru->chanPort = (pru->number)?31:30;   
    pru->chanName = "rpmsg_pru";
}

int PRU_setChannel(PRU* pru, int port, char* name){
    if(port < 0)
        return -EINVAL;
    pru->chanPort = port ;   
    pru->chanName = name;
    return 0;
}

State PRU_getState(PRU* pru){
    return pru->state;
}

int PRU_sendMsg(PRU* pru, char *message){
    char space[] = " "; 
    strcpy(command, "SENDMSG ");
    strcat(command, pru->chanName);
    strcat(command, space);
    //snprintf(command, sizeof(command), );
    char tmp[3];
    sprintf(tmp, "%d", pru->chanPort);
    strcat(command, tmp);
    strcat(command, space);
    strcat(command, message);
    //printf("DEBUG:%s\n", command);
    sendcmd(command);
} 

char* PRU_getMsg(PRU* pru){
    char space[] = " "; 
    strcpy(command, "GETMSG ");
    strcat(command, pru->chanName);
    strcat(command, space);
    //snprintf(command, sizeof(command), );
    char tmp[3];
    sprintf(tmp, "%d", pru->chanPort);
    strcat(command, tmp);
    //printf("DEBUG:%s\n", command);
    return sendcmd(command);
} 
