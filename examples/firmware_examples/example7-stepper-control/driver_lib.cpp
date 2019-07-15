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
    while(isMotorBusy == true){}

    this->p0.disable();
    this->p1.disable();

    this->p1.enable();
    this->p1.sendMsg_raw(to_string(10000));         
    this->p1.sendMsg_raw(to_string(20000));
    this->p1.sendMsg_raw(to_string(800));

    this->p0.enable();

    this->p.shutDown();

    this-> isMotorBusy = true;
    usleep(1000000);
    this-> isMotorBusy = false;
    return 0;
}

Driver::~Driver(){
    while(isMotorBusy == true){}
    this->p0.disable();
    this->p1.disable();
    this->p.shutDown();
}
