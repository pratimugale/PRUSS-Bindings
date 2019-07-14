#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"

#define PRU_SHARED 0x00010000

extern void start(void);

void main(void){
    start();
}
