#include "test.h"

int main(){
    PRUSS &pruss;
    PRU p0 = pruss.pru0;
    //char *ret = sendcmd("DISABLE_0");
    //printf("%s\n", ret);
    p0.number = 4;
    PRUSS_init(&pruss);

    printf("Num: %i\n", p0.number);
    return 0;
}
