#include <stdint.h>
#include <pru_cfg.h>
#include <math.h>
#include "resource_table_empty.h"

#define PRU_SHARED 0x00010000

static void* pruSharedMemory;
static uint8_t* pruSharedMemory_int;

extern void start(void);

void main(void){
    // Generate sine values in an array     
    
    int i;
    uint8_t waveform[100]; 
    float gain = 50.0f;
    float phase = 0.0f;
    float bias = 50.0f;
    float freq = 2.0f * 3.14159f / 100.0f;
    
    for (i = 0; i < 100; i++){
        waveform[i] = (uint8_t)(bias + (gain * sin((i*freq) + phase)));
        //waveform[i] = (uint8_t)i;
    }
    
    //uint16_t* sram_pointer = (uint16_t *) PRU_SHARED;
    pruSharedMemory = (void *) PRU_SHARED;

    pruSharedMemory_int = (uint8_t *)pruSharedMemory;

    uint8_t samplestep = 1;    //delay factor
    *(pruSharedMemory_int) = samplestep;

    uint8_t numbersamples = 100;
    *(pruSharedMemory_int+1) = numbersamples;

    for(i = 0; i < 100; i++){
        *(pruSharedMemory_int + 2 + i) = waveform[i];
    }

    start();
}
