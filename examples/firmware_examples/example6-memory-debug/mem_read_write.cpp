#include "../../../cpp-bindings/pruss.h"
#include <iostream>

using namespace std;

int main()
{
	PRUSS& p = PRUSS::get();
	PRU p0 = p.pru0;
	//p0.enable();
        int x;
        string offset;
        string data;

        // Choose which memory access is needed - read the enum from pruss.h
        Memory mem = SHARED;

        cout<<"1: Read; 2: Write; 0: Exit"<<endl;
        cin>>x;
        switch(x){
            case 1:
                cout<< "Enter address offset"<<endl;
                cin>>offset;
                cout<< "Data returned: "<<p0.mem_read(mem, offset)<<endl<<endl;
                break;
            case 2:
                cout<< "Enter address offset"<<endl;
                cin>>offset;
                cout<< "Enter data to be written"<<endl;
                cin >>data;
                cout<< "Return Value: "<<p0.mem_write(mem, offset, data)<<endl<<endl;
                break;
            default:
                break;
        }
//      p.bootUp();
//      p0.enable();
//      p.restart();
//	p1.disable();
//	p.shutDown();
}
