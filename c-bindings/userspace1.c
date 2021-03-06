#include <stdio.h>
#include "pruss.h"

int main(){
    PRUSS pruss;
    PRUSS_init(&pruss);
    PRU p1 = pruss.pru1;
    PRU_init(&p1, 1);
    PRU_enable(&p1);
    /*if(PRU_load(&p0, "/home/debian/work/my/PRUSS-Bindings/c/firmware_examples/rpmsg_echo/gen/rpmsg_echo.out")){
        printf("Firmware loaded\n");
    }
    else
	return -1;
        */

    char temp[50];
    
    while(true){
        printf("press 1 to send message; 2 to receive message(if any); 0 to break \n");
        int x;
        scanf("%d\n", &x);
        if (x == 1){
            printf("Enter a word to be sent via the RPMsg channel: ");
            fgets(temp, 50, stdin);

            PRU_sendMsg(&p1, temp);
            PRU_showRegs(&p1);
        }
        else if(x == 2){
            printf("\nReceived Message: %s\n", PRU_getMsg(&p1));
        }
        else if(x == 0){
            PRU_disable(&p1);
            PRUSS_shutDown(&pruss);
            return 0;
        }
    }

}
