#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"

#define PRU_SHARED 0x00010000

extern void start(void);

void main(void){
 //   CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    //volatile int* buffer = (volatile int *) PRU_SHARED;

    start();
}
