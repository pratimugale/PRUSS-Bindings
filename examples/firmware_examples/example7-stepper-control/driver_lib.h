#ifndef DRIVER_H_
#define DRIVER_H_

#include <stdlib.h>
#include <string>
#include <errno.h>
#include <unistd.h>
#include "../../../cpp-bindings/pruss.h"

using namespace std;

enum STEP_MODE{
    SINGLE,
    HALF,
    QUARTER,
    EIGHTH
};

class Driver{
    private:
        float degrees;
        bool isMotorBusy = false;

    public:
        Driver();
        void setDegrees(float degrees);
        float getDegrees();
        bool getIsMotorBusy();

        int activate_motor();

        PRUSS& p = PRUSS::get();
        PRU p0 = p.pru0;
        PRU p1 = p.pru1;
        ~Driver();

        static Driver& get();
};

#endif
