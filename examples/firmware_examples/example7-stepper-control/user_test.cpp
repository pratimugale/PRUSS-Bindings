#include "driver_lib.h"
#include <iostream>

using namespace std;

int main(){
    Driver& d = Driver::get();

    d.activateMotor(360.0*10, 60.0); // 180 degrees rotate at 30 RPM
   // d.activateMotor(540.0, 150.0); // 540 degrees rotate at 150 RPM
   // d.activateMotor(45.0, 1.0); // 45 degrees rotate at 120 RPM

    return 0;
}
