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
    PRU p0 = p.pru0;
    p0.disable();

    uint8_t noOfChannels = 4;

    uint8_t i;
    uint8_t waveform1[101]; 
    uint8_t waveform2[101]; 
    uint8_t waveform3[101]; 
    uint8_t waveform4[101]; 
    uint8_t waveform5[101]; 
    uint8_t waveform6[101]; 
    uint8_t waveform7[101]; 
    uint8_t waveform8[101]; 
    float gain = 50.0f;
    float phase1 = 0.0f;
    float phase2 = 1.0f * (float)PI/8;
    float phase3 = 2.0f * (float)PI/8;
    float phase4 = 3.0f * (float)PI/8;
    float phase5 = 4.0f * (float)PI/8;
    float phase6 = 5.0f * (float)PI/8;
    float phase7 = 6.0f * (float)PI/8;
    float phase8 = 7.0f * (float)PI/8;
    float phase9 = 8.0f * (float)PI/8;
    float bias = 50.0f;
    float freq1 = 2.0f * (float)PI / 50.0f;
    float freq2 = 2.0f * (float)PI / 75.0f;
    float freq = 2.0f * (float)PI / 100.0f;
    
    for (i = 0; i < 101; i++){

        if (i >= 50){
            waveform1[i] = (uint8_t)150;
        }
        
        else{
            waveform1[i] = (uint8_t)((bias + (gain * sin((i*freq1) + phase1))));
        }
    }

    for (i = 0; i < 101; i++){
        if ( i >= 100){
            waveform2[i] = (uint8_t)150;
        }

        else{
            waveform2[i] = (uint8_t)((bias + (gain * sin((i*freq) + phase1))));
            //waveform2[i] = (uint8_t)i*100/75;
        }
    }

    for (i = 0; i < 101; i++){
        if ( i >= 100){
            waveform3[i] = (uint8_t)150;
        }
        else
            waveform3[i] = (uint8_t)((bias + (gain * sin((i*freq) + phase9))));
    }
    
    for (i = 0; i < 101; i++){
        if ( i >= 100){
            waveform4[i] = (uint8_t)150;
        }
        else
            waveform4[i] = (uint8_t)((bias + (gain * sin((i*freq) + phase1))));
    }

    for (i = 0; i < 101; i++){
        if ( i >= 100){
            waveform5[i] = (uint8_t)150;
        }
        else
            waveform5[i] = (uint8_t)((bias + (gain * sin((i*freq) + phase1))));
    }

    for (i = 0; i < 101; i++){
        if ( i >= 100){
            waveform6[i] = (uint8_t)150;
        }
        else 
            waveform6[i] = (uint8_t)((bias + (gain * sin((i*freq) + phase1))));
    }

    for (i = 0; i < 101; i++){
        if ( i >= 100){
            waveform7[i] = (uint8_t)150;
        }
        else
            waveform7[i] = (uint8_t)((bias + (gain * sin((i*freq) + phase1))));
    }
    
    for (i = 0; i < 101; i++){
        if ( i >= 100){
            waveform8[i] = (uint8_t)150;
        }
        else
            waveform8[i] = (uint8_t)((bias + (gain * sin((i*freq) + phase1))));
    }

    // TODO************
    //uint8_t totalNoOfSamples = 100*noOfChannels;

    uint32_t totalNoOfSamples = 800;

    p1.sendMsg_raw(to_string(totalNoOfSamples));

    for(i = 0; i < 101; i++){
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

    p0.enable();

}
