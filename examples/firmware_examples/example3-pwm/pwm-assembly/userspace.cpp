#include "../../../../cpp-bindings/pruss.h"
#include <iostream>

using namespace std;

int main()
{
	PRUSS& p = PRUSS::get();
	PRU p1 = p.pru1;
        
        // Start PRU1 core which stores input values into PRU SRAM.
	p1.enable();            

        float frequency;
	cout << "Frequency of PWM in Hz (from 1Hz to 1MHz): "<<endl;
        cin>>frequency;
        float multiplier = 1000000.0/frequency;

        float duty_cycle;
        cout<< "Duty Cycle of PWM (0 to 1)"<<endl;
        cin>>duty_cycle;
        
        // Invalid Duty Cycle.
        if(duty_cycle > 1 || duty_cycle < 0){
            return -1;
        }

        // Base values for highest frequency of 1MHz.
        float on_samples = duty_cycle * 100;
        float total_samples = 100;

        // Adjusted values for frequencies less than 1MHz
        on_samples *= multiplier;
        total_samples *= multiplier;

        cout<<"On Cycles/2 = "<<(int)on_samples<<endl;
        cout<<"Total Cycles/2 = "<<(int)total_samples<<endl;

        // Write binary integers to the character device file.
	p1.sendMsg_raw(to_string((int)on_samples));         
	p1.sendMsg_raw(to_string((int)total_samples));

        // Start PRU0 core which executes the PWM assembly code.
        PRU p0 = p.pru0;
        p0.enable();

}
