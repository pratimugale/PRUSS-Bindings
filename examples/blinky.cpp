#include "../cpp-bindings/pruss.h"
#include <iostream>

using namespace std;

int main()
{
	PRUSS p;
	PRU p1 = p.pru1;
	p1.load("./firmware_examples/blinky/gen/blinky.out"); 
	cout << "Blinky loaded on PRU1(press p to pause, r to resume, d to disable, e to enable, q to quit)" << endl;
	char ch;
	cin >> ch;
	while(ch != 'q')
	{
		switch(ch){
			case 'p': cout << p1.pause() << endl;
				  break;
			case 'r': cout << p1.resume() << endl;
				  break;
			case 'd': cout << p1.disable() << endl;
				  break;
			case 'e': cout << p1.enable() << endl;
				  break;
			default : cout << "Invalid command" << endl;
		}
		cin >> ch;
	}
	p1.disable();
	p.shutDown();
}
