#include "driver_lib.h"
#include <iostream>

using namespace std;

int main(){
    Driver& d = Driver::get();

    d.wake();
    d.activateMotor(360.0*3, 60.0, FULL, ANTICLOCKWISE); // 180 degrees rotate at 30 RPM
    d.sleep();
    usleep(1000000);
    d.wake();
    d.activateMotor(360.0*3, 60.0); // 540 degrees rotate at 150 RPM
    d.activateMotor(180.0, 150.0, EIGHT, ANTICLOCKWISE); // 45 degrees rotate at 120 RPM

    return 0;
}
