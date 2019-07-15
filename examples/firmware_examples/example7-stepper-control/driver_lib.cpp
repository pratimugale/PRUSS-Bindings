#include "driver_lib.h"

using namespace std;

Driver& Driver::get(){
    static Driver driver;
    return driver;
}

Driver::Driver(){
}

void Driver::setDegrees(float degrees){
    this->degrees = degrees;
}

float Driver::getDegrees(){
    return this->degrees;
}

bool Driver::getIsMotorBusy(){
    return this->isMotorBusy;
}

int Driver::activate_motor(){

    // Wait until the Motor is done with its previous job.
    while(isMotorBusy == true){}

    this->p0.disable();
    this->p1.disable();

    // Start PRU p1 which is loaded with the RPMsg firmware - takes input frequency, dc and number of pulses to be generated.
    this->p1.enable();
    this->p1.sendMsg_raw(to_string(20000));     // Hardcoded on_cycles - Need to change these acc to freq, dc
    this->p1.sendMsg_raw(to_string(40000));     // Hardcoded off_cycles 
    this->p1.sendMsg_raw(to_string(800));       // Number of pulses to be sent - 800 pulses = 180 degree in eighth stepping mode.

    // Start PRU p0 which actually sends out the pulses through GPIO P9_31 all three above mentioned values are set.
    this->p0.enable();

    this-> isMotorBusy = true;
    // Wait till motor is done rotating.
    usleep(1000000);
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
