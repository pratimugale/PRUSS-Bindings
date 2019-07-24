#include "../../../cpp-bindings/pruss.h"
#include <iostream>
#include <math.h>

#define PI 3.14159

using namespace std;

int main()
{
    PRUSS& p = PRUSS::get();
    PRU p1 = p.pru1;
    p1.enable();            // Starts PRU1 (rpmsg_pru31 character device file is created here)

    uint8_t noOfChannels = 2;

    uint8_t i;
    uint8_t waveform1[100]; 
    uint8_t waveform2[100]; 
    float gain = 50.0f;
    float phase1 = 0.0f;
    float phase2 = 1.0f * (float)PI;
    float bias = 50.0f;
    float freq = 2.0f * (float)PI / 100.0f;
    
    for (i = 0; i < 100; i++){
        waveform1[i] = (uint8_t)(bias + (gain * sin((i*freq) + phase1)));
    }
    for (i = 0; i < 100; i++){
        waveform2[i] = (uint8_t)(bias + (gain * sin((i*freq) + phase2)));
    }

    
    uint8_t numbersamples = 100*noOfChannels;

    p1.sendMsg_raw(to_string(numbersamples));

    for(i = 0; i < 100; i++){
            p1.sendMsg_raw(to_string(waveform1[i]));
            p1.sendMsg_raw(to_string(waveform2[i]));
    }

    p1.disable();

    PRU p0 = p.pru0;
    p0.enable();

}
