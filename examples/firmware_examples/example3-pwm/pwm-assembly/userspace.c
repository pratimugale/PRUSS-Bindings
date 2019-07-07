#include "../../../../cpp-bindings/pruss.h"
#include <iostream>
#include <cmath>

using namespace std;

int main()
{
	PRUSS& p = PRUSS::get();
	PRU p0 = p.pru0;
	p0.enable();

        float x;
        int frequency;
        string offset;
        string data;

        cout<<"Frequency index: "<<endl;
        float index;
        cin >> index;
        float multiplier = 1/index;
        cout << multiplier<<endl;

        cout<<"Duty cycle(accurate to 2 decimal places)"<<endl;
        cin>>x;

        float max = 100*multiplier;
        x *= max;
        int dutyCycle = round(x);
        int maxCycle  = round(max);
        dutyCycle *= multiplier;

        //cout <<"Current Duty Cycle: "+dutyCycle<<endl;

        // Passing duty cycle to the PRU at local location 0x00010000, 0x00010001
        Memory mem = SHARED;
        char duty_Cycle[10];
        char max_Cycle[10];
        sprintf(duty_Cycle, "%x", dutyCycle);             //Convert to hexadecimal
        cout<<duty_Cycle<<endl;
        p0.mem_write(mem, "0x00000000", duty_Cycle);      
        sprintf(max_Cycle, "%x", maxCycle);               //Convert to hexadecimal
        p0.mem_write(mem, "0x00000001", max_Cycle);

	PRU p0 = p.pru0;
	p0.enable();

        return 0;
}
