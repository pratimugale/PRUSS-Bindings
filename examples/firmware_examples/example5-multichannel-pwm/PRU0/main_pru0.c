#include <stdint.h>
#include <pru_cfg.h>
#include <math.h>
#include "resource_table_empty.h"

#define PRU_SHARED 0x00010000

static void *pruSharedMemory;
static uint8_t *pruSharedMemory_uint8;

extern void setup(void);

void main(void){

    setup();
    // Write the duty cycles manually into the PRU SRAM. (Should later be done using RPMsg.)
 
//    int i;
//    uint8_t values[40]; 

    // Duty Cycle values of the channels out of 10.
    uint8_t duty1 = 1; //10% DC
    uint8_t duty2 = 4; //40% DC
    uint8_t duty3 = 5; //50% DC
    uint8_t duty4 = 9; //90% DC
    
/*    for (i = 0; i < 40; i++){
        if (i%4 == 0){
            if (duty1 > 0){
                values[i] = 1;
                duty1--;
            }
            else
                values[i] = 0;
        }
        if (i%4 == 1){
            if (duty2 > 0){
                values[i] = 1;
                duty2--;
            }
            else
                values[i] = 0;
        }
        if (i%4 == 2){
            if (duty3 > 0){
                values[i] = 1;
                duty3--;
            }
            else
                values[i] = 0;
        }
        if (i%4 == 3){
            if (duty4 > 0){
                values[i] = 1;
                duty4--;
            }
            else
                values[i] = 0;
        }
    }*/
    
    //uint16_t* sram_pointer = (uint16_t *) PRU_SHARED;
    pruSharedMemory = (void *) PRU_SHARED;

    pruSharedMemory_uint8 = (uint8_t *)pruSharedMemory;

   /* for(i = 0; i < 40; i++){
        *(pruSharedMemory_uint8 + i) = values[i];
    }*/

    *(pruSharedMemory_uint8) = duty1;
    *(pruSharedMemory_uint8 + 1) = duty2;
    *(pruSharedMemory_uint8 + 2) = duty3;
    *(pruSharedMemory_uint8 + 3) = duty4;

    //start();
}
