#include "../cpp-bindings/pruss.h"
#include <iostream>

using namespace std;

int main()
{
	PRUSS& p = PRUSS::get();
	PRU p0 = p.pru0;
	p0.enable();
	if(!p0.load("./firmware_examples/rpmsg_echo/gen/rpmsg_echo.out"))
		cout << "Firmware loaded\n";
	else
		return -1;
	string s;
	cout << "Enter a message: ";
	getline(cin, s);
	p0.sendMsg(s);
	cout << "Loopback       : "<< p0.getMsg();
	p0.disable();
	p.shutDown();

}
