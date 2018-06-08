#include "../cpp-bindings/pruss.h"
#include <iostream>

using namespace std;

int main()
{
	PRUSS p;
	PRU p1 = p.pru1;
	if(p1.load("./firmware_examples/pru_pin_state_reader/gen/pru_pin_state_reader.out"))
		return 0;
	p1.sendMsg("S");
	int timeout, ret;
	cout << "Enter timeout(in seconds)(-1 for indefinite): ";
	cin >> timeout;
	cout << "Waiting for state change on pin 8.45...." << "\n";
	if(timeout == -1)
		ret = p1.waitForEvent();
	else
		ret = p1.waitForEvent(timeout);
	
	cout << (ret?"Timer Expired":"Event occured") << "\n";
	p1.disable();
	p.shutDown();

}
