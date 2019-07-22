#include <stdint.h>
#include <pru_cfg.h>
#include <math.h>
#include "resource_table_empty.h"

#define PRU_DATA0 0x00000000

extern void start(void);
volatile uint8_t* data0_pointer = (volatile uint8_t *)PRU_DATA0;

void main(void){

    start();
    // Generate sine values in an array     
/*    
    uint8_t i;
    uint8_t waveform[100]; 
    float gain = 50.0f;
    float phase = 0.0f;
    float bias = 50.0f;
    float freq = 2.0f * 3.14159f / 100.0f;
    
    for (i = 0; i < 100; i++){
        waveform[i] = (uint8_t)(bias + (gain * sin((i*freq) + phase)));
        //waveform[i] = i;
    }
    
    // ** DELAY FACTOR HAS YET NOT BEEN IMPLEMENTED IN THE ASSEMBLY PROGRAM.
    uint8_t samplestep = 1;    //delay factor
 //   *(data0_pointer) = samplestep;

    uint8_t numbersamples = 100;
 //   *(data0_pointer+1) = numbersamples;

    for(i = 0; i < 100; i++){
        *(data0_pointer + 2 + i) = waveform[i];
        __delay_cycles(10000);
    }
*/
}
