#include "../cpp-bindings/pruss.h"
#include <iostream>

using namespace std;

int main()
{
	PRUSS& p = PRUSS::get();
	PRU p1 = p.pru1;
	p1.enable();
	/*if(!p0.load("./firmware_examples/rpmsg_echo/gen/rpmsg_echo.out"))
		cout << "Firmware loaded\n";
	else
		return -1;*/
	string s;
	cout << "Enter a message: ";
	getline(cin, s);
	p1.sendMsg_string(s);
	cout << "Loopback       : "<< p1.getMsg();
	p1.disable();
	p.shutDown();

}
