#include <stdint.h>
#include <pru_cfg.h>
#include <math.h>
#include "resource_table_empty.h"

#define PRU_DATA0 0x00010000
#define PI 3.14159

extern void start(void);
volatile uint16_t* sram_pointer = (volatile uint16_t *)PRU_DATA0;

void main(void){

    //*(sram_pointer) = (uint16_t) 2000;

    start();

    // Generate sine values in an array     
    
 /*   uint8_t i;
    uint8_t waveform1[100]; 
    uint8_t waveform2[100]; 
    float gain = 50.0f;
    float phase1 = 0.0f;
    float phase2 = (float)PI;
    float bias = 50.0f;
    float freq = 2.0f * (float)PI / 100.0f;
    
    for (i = 0; i < 100; i++){
        waveform1[i] = (uint8_t)(bias + (gain * sin((i*freq) + phase1)));
        //waveform[i] = i;
    }

    for (i = 0; i < 100; i++){
        waveform2[i] = (uint8_t)(bias + (gain * sin((i*freq) + phase2)));
        //waveform[i] = i;
    }
    
    // ** DELAY FACTOR HAS YET NOT BEEN IMPLEMENTED IN THE ASSEMBLY PROGRAM.
//    uint8_t samplestep = 1;    //delay factor
    /(data0_pointer) = samplestep;

    uint8_t numbersamples = 100;
    *(data0_pointer) = numbersamples;

    for(i = 0; i < 100; i++){
        *(data0_pointer + 2 + i) = waveform1[i];
        __delay_cycles(10000);
    }

    *(data0_pointer + 2) = samplestep;

    *(data0_pointer + 1) = (uint8_t)numbersamples;

    for(i = 0; i < 100; i++){
        *(data0_pointer + 103 + i) = waveform2[i];
        __delay_cycles(10000);
    }
*/
}
