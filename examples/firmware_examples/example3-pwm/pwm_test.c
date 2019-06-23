#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;
int i;

void stall(int n){
    for (i = 0; i < n; i++){
        __delay_cycles(1);
    }
}

int calcCycles_D(float d, int offCycles){
    int cycles;
    if (d < 1){
        cycles = (int)((offCycles * d)/(1 - d));
        return cycles;
    }
    else
        return 0;
}

int calcCycles_V(float v, int offCycles){
    float duty_cycle = v/3.14;
    int cycles;
    if(duty_cycle < 1)
        cycles = calcCycles_D(duty_cycle, offCycles);
    else 
        return 0;
    return cycles;
}

void main(void){
    uint32_t gpio;

    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    gpio = 0x0001;

    int offCycles = 10000; 
    int onCycles = calcCycles_V(2, offCycles);

    while(1){
        __R30 |= gpio;
        stall(onCycles);
        __R30 &= ~gpio;
        stall(offCycles);
    }
    
}
