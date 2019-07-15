#ifndef DRIVER_H_
#define DRIVER_H_

#include <stdlib.h>
#include <string>
#include <errno.h>
#include <unistd.h>
#include "../../../cpp-bindings/pruss.h"

using namespace std;

enum Direction{
    CLOCKWISE,
    ANTICLOCKWISE
};

enum StepMode{
    SINGLE  = 200,
    HALF    = 400,
    QUARTER = 800,
    EIGHT   = 1600
};

class Driver{
    private:
        float degrees;
        float rpm;
        bool isMotorBusy = false;
        StepMode stepMode;
        Direction direction;
        bool asleep;
        int onCycles;
        int totalCycles;
        int noOfPulses;

    public:
        Driver();

        //void setDegrees(float degrees);
        //float getDegrees();
        void setStepMode(StepMode stepMode);
        StepMode getStepMode();
        void setDirection(Direction direction);
        Direction getDirection();
        bool getIsMotorBusy();

        void calculateCycles(float degrees, float rpm);
        int activateMotor(float degrees = 360.0, float rpm = 60.0, StepMode stepMode = EIGHT);
        void sleep();
        void wake();
        bool isAsleep();

        PRUSS& p = PRUSS::get();
        PRU p0 = p.pru0;
        PRU p1 = p.pru1;

        ~Driver();

        static Driver& get();
};

#endif
