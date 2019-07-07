#include "../../../../cpp-bindings/pruss.h"
#include <iostream>

using namespace std;

int main()
{
	PRUSS& p = PRUSS::get();
	PRU p1 = p.pru1;
	p1.enable();            // Starts PRU1 (rpmsg_pru31 character device file is created here)

	string s;
        float frequency;
	cout << "Frequency of PWM in Hz (from 1Hz to 1MHz): "<<endl;
        cin>>frequency;
        float multiplier = 1000000.0/frequency;

        float duty_cycle;
        cout<< "Duty Cycle of PWM "<<endl;
        cin>>duty_cycle;

        float on_samples = duty_cycle * 100;
        float total_samples = 100;

        on_samples *= multiplier;
        total_samples *= multiplier;

        cout<<"On Cycles/2 = "<<(int)on_samples<<endl;
        cout<<"Total Cycles/2 = "<<(int)total_samples<<endl;

	p1.sendMsg_raw(to_string((int)on_samples));
        p1.getMsg();
	p1.sendMsg_raw(to_string((int)total_samples));
        p1.getMsg();

        PRU p0 = p.pru0;
        p0.enable();

}
