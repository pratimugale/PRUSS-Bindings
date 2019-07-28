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

    uint8_t noOfChannels = 4;

    uint8_t i;
    uint8_t waveform1[100]; 
    uint8_t waveform2[100]; 
    uint8_t waveform3[100]; 
    uint8_t waveform4[100]; 
    uint8_t waveform5[100]; 
    uint8_t waveform6[100]; 
    uint8_t waveform7[100]; 
    uint8_t waveform8[100]; 
    float gain = 50.0f;
    float phase1 = 0.0f;
    float phase2 = 1.0f * (float)PI/8;
    float phase3 = 2.0f * (float)PI/8;
    float phase4 = 3.0f * (float)PI/8;
    float phase5 = 4.0f * (float)PI/8;
    float phase6 = 5.0f * (float)PI/8;
    float phase7 = 6.0f * (float)PI/8;
    float phase8 = 7.0f * (float)PI/8;
    float bias = 50.0f;
    float freq = 2.0f * (float)PI / 100.0f;
    
    for (i = 0; i < 100; i++){
        waveform1[i] = (uint8_t)((bias + (gain * sin((i*freq) + phase1))));
    }

    for (i = 0; i < 100; i++){
        waveform2[i] = (uint8_t)((bias + (gain * sin((i*freq) + phase2))));
    }

    for (i = 0; i < 100; i++){
        waveform3[i] = (uint8_t)((bias + (gain * sin((i*freq) + phase3))));
    }
    
    for (i = 0; i < 100; i++){
        waveform4[i] = (uint8_t)((bias + (gain * sin((i*freq) + phase4))));
    }

    for (i = 0; i < 100; i++){
        waveform5[i] = (uint8_t)((bias + (gain * sin((i*freq) + phase5))));
    }

    for (i = 0; i < 100; i++){
        waveform6[i] = (uint8_t)((bias + (gain * sin((i*freq) + phase6))));
    }

    for (i = 0; i < 100; i++){
        waveform7[i] = (uint8_t)((bias + (gain * sin((i*freq) + phase7))));
    }
    
    for (i = 0; i < 100; i++){
        waveform8[i] = (uint8_t)((bias + (gain * sin((i*freq) + phase8))));
    }

    // TODO************
    //uint8_t totalNoOfSamples = 100*noOfChannels;

    uint32_t totalNoOfSamples = 800;

    p1.sendMsg_raw(to_string(totalNoOfSamples));

    for(i = 0; i < 100; i++){
            p1.sendMsg_raw(to_string(waveform1[i]));
            p1.sendMsg_raw(to_string(waveform2[i]));
            p1.sendMsg_raw(to_string(waveform3[i]));
            p1.sendMsg_raw(to_string(waveform4[i]));
            p1.sendMsg_raw(to_string(waveform5[i]));
            p1.sendMsg_raw(to_string(waveform6[i]));
            p1.sendMsg_raw(to_string(waveform7[i]));
            p1.sendMsg_raw(to_string(waveform8[i]));
    }

    p1.disable();

    PRU p0 = p.pru0;
    p0.enable();

}
