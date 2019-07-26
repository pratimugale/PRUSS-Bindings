#include <stdint.h>
#include <pru_cfg.h>
#include <math.h>
#include "resource_table_empty.h"

extern void start(void);

void main(void){

    // Start the execution of the pulse-supply.asm prgram endlessly
    start();
    
}
