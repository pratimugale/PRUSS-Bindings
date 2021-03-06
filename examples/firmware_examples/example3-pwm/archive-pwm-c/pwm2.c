#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#define PRU_SHARED 0x00010000

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
    if (d == 1){
        return -1;  
    }
    else
        return -2;
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
    
    volatile int* buffer = (volatile int *) PRU_SHARED;

    gpio = 0x0001;

    int offCycles = 10000; 

    while(1){
        int onCycles = calcCycles_D(0.1*buffer[0], offCycles);
        if (onCycles == -1){
            __R30 |= gpio;
        }
        else{
            __R30 |= gpio;
            stall(onCycles);
            __R30 &= ~gpio;
            stall(offCycles);
        }
    }
}
