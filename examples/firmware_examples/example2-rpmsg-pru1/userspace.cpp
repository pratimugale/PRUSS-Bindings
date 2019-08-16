#include <iostream>
#include "pruss.h"

using namespace std;

int main()
{
	PRUSS& p = PRUSS::get();
	PRU p1 = p.pru1;
	p1.enable();

	string s;
	cout << "Enter a message        : ";
	getline(cin, s);
	p1.sendMsg_string(s);

	cout << "Message from PRU       : "<< p1.getMsg();
	p1.disable();
	p.shutDown();

}
