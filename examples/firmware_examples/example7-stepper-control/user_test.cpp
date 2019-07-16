#include "driver_lib.h"
#include <iostream>

using namespace std;

int main(){
    Driver& d = Driver::get();

    d.activateMotor(180.0, 30.0); // 180 degrees rotate 
    d.activateMotor(540.0, 150.0); // 540 degrees rotate 
    d.activateMotor(45.0, 120.0); // 45 degrees rotate 

    return 0;
}
