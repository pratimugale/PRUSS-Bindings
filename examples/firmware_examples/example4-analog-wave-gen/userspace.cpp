#include "../../../cpp-bindings/pruss.h"
#include <iostream>
#include <math.h>

using namespace std;

int main()
{
    PRUSS& p = PRUSS::get();
    PRU p1 = p.pru1;
    p1.enable();            // Starts PRU1 (rpmsg_pru31 character device file is created here)


    uint8_t i;
    uint8_t waveform[100]; 
    float gain = 50.0f;
    float phase = 0.0f;
    float bias = 50.0f;
    float freq = 2.0f * 3.14159f / 100.0f;
    
    for (i = 0; i < 100; i++){
        waveform[i] = (uint8_t)(bias + (gain * sin((i*freq) + phase)));
    }
    
    uint8_t samplestep = 1;    //delay factor
    uint8_t numbersamples = 100;

    p1.sendMsg_raw(to_string(samplestep));
    p1.sendMsg_raw(to_string(numbersamples));

    for(i = 0; i < 100; i++){
       p1.sendMsg_raw(to_string(waveform[i]));
    }

    p1.disable();

    PRU p0 = p.pru0;
    p0.enable();

}
