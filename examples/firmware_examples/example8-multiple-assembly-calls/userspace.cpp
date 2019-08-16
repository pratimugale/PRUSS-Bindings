#include "pruss.h"

using namespace std;

int main(){
    PRUSS& p = PRUSS::get();
    
    PRU p0 = p.pru0;

    p0.enable();

    return 0;
}
