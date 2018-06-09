#include "../cpp-bindings/pruss.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main()
{
	int status = 0, in;
	PRUSS& p = PRUSS::get();
	PRU p1 = p.pru1;
	PRU p0 = p.pru0;
	p1.load("./firmware_examples/multi_threaded/gen/main_pru1_fw.out");
	p0.load("./firmware_examples/multi_threaded/gen/main_pru0_fw.out");
	p1.pause();
	p0.sendMsg("s");
	int pid = fork(); // event wait in child
	if(pid > 0)
	{	
		cin >> in;
		cout << "PRU1 resumed:" << p1.resume() <<  "\n";
		cout << "Waiting for Child\n";
		wait(&status);
		cout << "Wait over for Child\n";
		
		p1.disable();
		p0.disable();
		p.shutDown();

	}
	else if(pid == 0)
	{
		cout << "Wait Start on channel 0\n";
		cout <<  p0.waitForEvent(100) << "\n";
		cout <<  p0.getMsg();
		cout << "Wait over on channel 0\n";
		exit(0);	
	}
	else
		cout << "fork failed\n";

}

