#include "../cpp-bindings/pruss.h"
#include <iostream>

using namespace std;

int main()
{
	PRUSS& p = PRUSS::get();
	PRU p1 = p.pru1;
	p1.enable();
	/*if(!p1.load("./firmware_examples/rpmsg_echo/gen/rpmsg_echo.out"))
		cout << "Firmware loaded\n";
	else
		return -1;*/
        //p1.mem_read();

        cout<< "Enter address offset"<<endl;
        string offset;
        cin>>offset;
        Memory mem = SHARED;
        cout << mem<<endl;
        cout<< "Test: "<<p1.mem_write(mem, offset, 14)<<endl<<endl;
	p1.disable();
	p.shutDown();
}
