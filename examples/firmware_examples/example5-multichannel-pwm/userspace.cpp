#include "../../../cpp-bindings/pruss.h"
#include <iostream>

using namespace std;

int main()
{
	PRUSS& p = PRUSS::get();
	PRU p1 = p.pru1;
	p1.enable();            // Starts PRU1 (rpmsg_pru31 character device file is created here)

        // Input duty cycle is accurate to 2 decimal places.
        float dc[4];
        for (int i = 0; i < 4; i++){
	    cout << "Duty Cycle" << i << ": in ((DC%)/100) form"<<endl;
            cin>>dc[i];
        }

        float multiplier = 100;

        float on_samples[4];
        float off_samples[4];

        for (int i = 0; i < 4; i++){
            if (dc[i] > 0 && dc[i] < 1){
                on_samples[i] = multiplier * dc[i];
                off_samples[i] = 100 - (int)on_samples[i];
            }
            
            else{
                return -1;
            }
        }

        for (int i = 0; i < 4; i++){
	    p1.sendMsg_raw(to_string((int)on_samples[i]));
            p1.getMsg();
	    p1.sendMsg_raw(to_string((int)off_samples[i]));
            p1.getMsg();
        }

        PRU p0 = p.pru0;
        p0.enable();

}
