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

        int x;
        string offset;
        int data;
        Memory mem = SHARED;

        cout<<"1: Read; 2: Write; 0: Exit"<<endl;
        cin>>x;
        switch(x){
            case 1:
                cout<< "Enter address offset"<<endl;
                cin>>offset;
                cout<< "Data returned: "<<p1.mem_read(mem, offset)<<endl<<endl;
                break;
            case 2:
                cout<< "Enter address offset"<<endl;
                cin>>offset;
                cout<< "Enter data to be written"<<endl;
                cin >>data;
                cout<< "Return Value: "<<p1.mem_write(mem, offset, data)<<endl<<endl;
                break;
            default:
                break;
        }
	p1.disable();
	p.shutDown();
}
