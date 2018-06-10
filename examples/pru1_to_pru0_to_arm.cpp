#include "../cpp-bindings/pruss.h"
#include <iostream>
#include <sys/time.h>

using namespace std;

int main()
{
	struct timeval before, after;
	PRUSS& p = PRUSS::get();
	PRU p0 = p.pru0;
	PRU p1 = p.pru1;
	p0.load("./firmware_examples/pru1_to_pru0_to_arm/gen/main_pru0_fw.out");
	p1.load("./firmware_examples/pru1_to_pru0_to_arm/gen/main_pru1_fw.out");
	
	p0.sendMsg("s"); //kick

	for(int i = 0; i<10 ; i++)
	{
		gettimeofday(&before, NULL);
		if(p0.waitForEvent(10))
			return 0;
		cout << p0.getMsg();
		gettimeofday(&after, NULL);
		cout << ((after.tv_sec - before.tv_sec)*1000000L + after.tv_sec) - before.tv_sec << "\n";
	}
	p1.disable();
	p0.disable();


}
