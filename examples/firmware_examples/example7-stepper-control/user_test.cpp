#include "driver_lib.h"
#include <iostream>

using namespace std;

int main(){
    Driver& d = Driver::get();

    d.activate_motor(); // 180 degrees rotate 
    d.activate_motor(); // 180 degrees rotate 
    d.activate_motor(); // 180 degrees rotate 

    return 0;
}
