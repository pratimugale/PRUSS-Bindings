#include "driver_lib.h"

#include <iostream>

using namespace std;

Driver& Driver::get(){
    static Driver driver;
    return driver;
}

Driver::Driver(){
    // Default microstepping mode = eighth step mode - One rotation is 1600 steps
    //                              quarter step mode- One rotation is 800  steps
    //                              half step mode   - One rotation is 400  steps
    //                              full step mode   - One rotation is 200  steps
    
    setStepMode(EIGHT);
    // Set default direction to clockwise.
}

void Driver::setStepMode(StepMode stepMode){
    this->stepMode = stepMode;
    // Adjust GPIO output here.
}

bool Driver::getIsMotorBusy(){
    return this->isMotorBusy;
}

void Driver::calculateCycles(float degrees, float rpm){
    float stepsPerRevolution = (float)this->stepMode;
    this->noOfPulses = (int)((stepsPerRevolution * degrees)/360.0);
    float frequency = ((stepsPerRevolution * rpm)/60.0);
    float multiplier = ((float)MAX_FREQUENCY)/frequency; 
    this->onCycles = (int)(0.5 * 100 * multiplier); 
    this->totalCycles = (int)(100 * multiplier); 
}

int Driver::activateMotor(float degrees, float rpm, StepMode stepMode){
    // Calculate the values beforehand if the motor is already in running state
    setStepMode(stepMode);
    calculateCycles(degrees, rpm);

    // Wait until the Motor is done with its previous job.
    while(isMotorBusy == true){}

    this->p0.disable();
    this->p1.disable();

    // Start PRU p1 which is loaded with the RPMsg firmware - takes input frequency, dc and number of pulses to be generated.
    this->p1.enable();
    this->p1.sendMsg_raw(to_string(this->onCycles));     // Hardcoded on_cycles - Need to change these acc to freq, dc
    this->p1.sendMsg_raw(to_string(this->totalCycles));     // Hardcoded total_cycles 
    this->p1.sendMsg_raw(to_string(this->noOfPulses));       // Number of pulses to be sent - 800 pulses = 180 degree in eighth stepping mode.

    // Start PRU p0 which actually sends out the pulses through GPIO P9_31 all three above mentioned values are set.
    this->p0.enable();

    this-> isMotorBusy = true;
    // Wait till motor is done rotating.
    // Here wait on event on RPMsg channel
//    usleep(1000000);
    string messageFromPRU;
    while(1){
        messageFromPRU = this->p1.getMsg();
        if(messageFromPRU.compare(EXPECTED_MESSAGE) == 0){
            cout<<"Motor Command Successful"<<endl;
            break;
        }
        
    }
    this-> isMotorBusy = false;
    return 0;
}

Driver::~Driver(){
    // Wait till the Motor completes its rotation after the program has ended
    while(isMotorBusy == true){}

    this->p0.disable();
    this->p1.disable();
    this->p.shutDown();
}
