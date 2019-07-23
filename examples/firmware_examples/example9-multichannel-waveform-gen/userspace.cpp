#include "../../../cpp-bindings/pruss.h"
#include <iostream>
#include <math.h>

using namespace std;

int main()
{
	PRUSS& p = PRUSS::get();
	PRU p1 = p.pru1;
	p1.enable();            // Starts PRU1 (rpmsg_pru31 character device file is created here)


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

    uint8_t samplestep = 1;    //delay factor
    uint8_t numbersamples = 130;

    p1.mem_write(DATA0, to_string(0), to_string(samplestep));
    p1.mem_write(DATA0, to_string(1), to_string(numbersamples));

    for(int i = 0; i < 100; i++){
        p1.mem_write(DATA0, to_string(2+i), to_string(i));
    }

   /* p1.sendMsg_raw(to_string((uint8_t)samplestep));

    uint8_t numbersamples = 130;
    p1.sendMsg_raw(to_string((uint8_t)numbersamples));

    for(i = 0; i < 100; i++){
      //  *(sram_pointer + 2 + i) = (uint8_t)(bias + (gain * sin((i*freq) + phase)));
       // *(sram_pointer + 2 + i) = (uint8_t)(i);
       // __delay_cycles(10000);
       p1.sendMsg_raw(to_string((uint8_t)waveform[i]));
    }

	//p1.sendMsg_raw(to_string((int)on_samples));
        //p1.getMsg();
	//p1.sendMsg_raw(to_string((int)total_samples));
        //p1.getMsg();
*/
        PRU p0 = p.pru0;
        p0.enable();

}
