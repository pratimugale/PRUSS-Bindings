#include "driver_lib.h"
#include <iostream>

using namespace std;

int main(){
    Driver& d = Driver::get();

    d.wake();
    d.activateMotor(360.0, 60.0, FULL, ANTICLOCKWISE);    // 360 degrees rotate at 60 RPM in Full Step mode, Anticlockwise direction
    d.activateMotor(360.0, 60.0, HALF, ANTICLOCKWISE); 
    d.activateMotor(360.0, 60.0, QUARTER, ANTICLOCKWISE); 
    d.activateMotor(360.0, 60.0, EIGHT, ANTICLOCKWISE); 
    d.activateMotor(360.0, 60.0, EIGHT);                  // 360 degrees rotate at 60 RPM in 1/8th Step Mode, Clockwise direction
    d.activateMotor(360.0, 60.0, QUARTER); 
    d.activateMotor(360.0, 60.0, HALF); 
    d.activateMotor(360.0, 60.0, FULL); 
    // To demonstrate sleeping - The motor should be able to freely rotate for 10 seconds
    d.sleep();
    usleep(5000000);
    d.wake();
    d.activateMotor(180.0, 150.0, EIGHT, ANTICLOCKWISE); 
    d.activateMotor(180.0, 150.0, EIGHT, CLOCKWISE); 
    d.activateMotor(180.0, 150.0, EIGHT, ANTICLOCKWISE); 
    d.activateMotor(180.0, 150.0, EIGHT, CLOCKWISE); 
    d.activateMotor(180.0, 150.0, EIGHT, ANTICLOCKWISE); 
    d.activateMotor(180.0, 150.0, EIGHT, CLOCKWISE); 
    d.activateMotor(180.0, 150.0, EIGHT, ANTICLOCKWISE); 
    d.activateMotor(180.0, 150.0, EIGHT, CLOCKWISE); 
    d.sleep();
    usleep(5000000);
    d.wake();
    d.activateMotor(360*150.0, 150.0, EIGHT, ANTICLOCKWISE); // To demonstrate RPM accuracy for 150 rotations.

    return 0;
}
