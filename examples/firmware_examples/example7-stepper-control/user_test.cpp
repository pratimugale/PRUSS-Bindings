#include "driver_lib.h"
#include <iostream>

using namespace std;

int main(){
    Driver d = Driver::get();
    d.setDegrees(140);
    cout<<d.getDegrees()<<endl;

    d.activate_motor();
    cout<<"press 1"<<endl;
    int x;
    cin>>x;
    d.activate_motor();

    return 0;
}
