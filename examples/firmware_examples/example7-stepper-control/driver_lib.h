#ifndef DRIVER_H_
#define DRIVER_H_

#include <stdlib.h>
#include <string>
#include <errno.h>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>
#include "../../../cpp-bindings/pruss.h"

#define MAX_FREQUENCY 1000000.0
#define EXPECTED_MESSAGE "done\n"

using namespace std;

enum Direction{
    CLOCKWISE     = 0,
    ANTICLOCKWISE = 1
};

enum StepMode{
    // Microstepping modes and their equivalent "steps per rotation" aka "pulses per rotation" they consume.
    FULL    = 200,
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

        void setStepMode(StepMode stepMode);
        StepMode getStepMode();
        void setDirection(Direction direction);
        Direction getDirection();
        bool getIsMotorBusy();

        void calculateCycles(float degrees, float rpm);
        int activateMotor(float degrees = 360.0, float rpm = 60.0, StepMode stepMode = EIGHT, Direction direction = CLOCKWISE);
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
