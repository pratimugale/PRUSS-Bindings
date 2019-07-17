#include "driver_lib.h"

#include <iostream>

using namespace std;

Driver& Driver::get(){
    static Driver driver;
    return driver;
}

Driver::Driver(){
    // Set default stepmode to:     eighth step mode - One rotation is 1600 steps
    //                              quarter step mode- One rotation is 800  steps
    //                              half step mode   - One rotation is 400  steps
    //                              full step mode   - One rotation is 200  steps
    setStepMode(EIGHT);

    // Set default direction to clockwise.
    setDirection(CLOCKWISE);
}

void Driver::setStepMode(StepMode stepMode){
    this->stepMode = stepMode;
    
    char* x;
    char* y;
    if (stepMode == FULL){
        x = (char*)"0";
        y = (char*)"0";
    }
    else if (stepMode == HALF){
        x = (char*)"0"; 
        y = (char*)"1";
    }
    else if (stepMode == QUARTER){
        x = (char*)"1";
        y = (char*)"0";
    }
    else if (stepMode == EIGHT){
        x = (char*)"1";
        y = (char*)"1";
    }

    // Set appropriate GPIOs for Microstepping.
    int fd1, fd2;
    char buf1[50], buf2[50];
    snprintf(buf1, sizeof(buf1), "/sys/class/gpio/gpio48/direction");
    snprintf(buf2, sizeof(buf2), "/sys/class/gpio/gpio49/direction");

    fd1 = open(buf1, O_WRONLY);
    if (fd1 < 0){
        perror("gpio/direction");
    }
    fd2 = open(buf2, O_WRONLY);
    if (fd2 < 0){
        perror("gpio/direction");
    }
    
    write(fd1, "out", 4);
    write(fd2, "out", 4);
    
    snprintf(buf1, sizeof(buf1), "/sys/class/gpio/gpio48/value");
    snprintf(buf2, sizeof(buf2), "/sys/class/gpio/gpio49/value");

    fd1 = open(buf1, O_WRONLY);
    if (fd1 < 0){
        perror("gpio/value");
    }
    fd2 = open(buf2, O_WRONLY);
    if (fd2 < 0){
        perror("gpio/value");
    }
    
    write(fd1, x, 2);
    write(fd2, y, 2);

    close(fd1);
    close(fd2);
}

StepMode Driver::getStepMode(){
    return this->stepMode;
}

void Driver::setDirection(Direction direction){
    this->direction = direction;

    // Set GPIO to DIR of driver motor
    int fd;
    char buf[50];
    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio115/direction");

    fd = open(buf, O_WRONLY);
    if (fd < 0){
        perror("gpio/direction");
    }
    
    write(fd, "out", 4);
    
    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio115/value");

    fd = open(buf, O_WRONLY);
    if (fd < 0){
        perror("gpio/value");
    }

    if (direction == 0){
        write(fd, "0", 2);
    }
    else {
        write(fd, "1", 2);
    }

    close(fd);
}

Direction Driver::getDirection(){
    return this->direction;
}

bool Driver::getIsMotorBusy(){
    return this->isMotorBusy;
}

void Driver::calculateCycles(float degrees, float rpm){
    float stepsPerRevolution = (float)this->stepMode;
    this->noOfPulses = (int)((stepsPerRevolution * degrees)/360.0);
    float frequency = ((stepsPerRevolution * rpm)/60.0);
    float multiplier = ((float)MAX_FREQUENCY)/frequency; 
    this->onCycles = (int)(0.5 * 100 * multiplier); 
    this->totalCycles = (int)(100 * multiplier); 
}

int Driver::activateMotor(float degrees, float rpm, StepMode stepMode, Direction direction){
    // Calculate the values beforehand if the motor is already in running state
    setStepMode(stepMode);
    setDirection(direction);
    calculateCycles(degrees, rpm);

    // Wait until the Motor is done with its previous job.
    while(isMotorBusy == true){}

    this->p0.disable();
    this->p1.disable();

    // Start PRU p1 which is loaded with the RPMsg firmware - takes input frequency, dc and number of pulses to be generated.
    this->p1.enable();
    this->p1.sendMsg_raw(to_string(this->onCycles));     
    this->p1.sendMsg_raw(to_string(this->totalCycles));  
    this->p1.sendMsg_raw(to_string(this->noOfPulses)); 

    // Start PRU p0 which actually sends out the pulses through GPIO P9_31 all three above mentioned values are set.
    this->p0.enable();

    this-> isMotorBusy = true;
    // Wait till motor is done rotating.
    // Here wait on event on RPMsg channel
    // usleep(1000000);
    string messageFromPRU;
    while(1){
        messageFromPRU = this->p1.getMsg();
        if(messageFromPRU.compare(EXPECTED_MESSAGE) == 0){
            cout<<"Motor Command Successful"<<endl;
            break;
        }
        
    }
    this-> isMotorBusy = false;
    return 0;
}

void Driver::sleep(){
    this->asleep = true; 
    // Set GPIO 
    
    int fd;
    char buf[50];
    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio117/direction");

    fd = open(buf, O_WRONLY);
    if (fd < 0){
        perror("gpio/direction");
    }
    
    write(fd, "out", 4);
    
    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio117/value");

    fd = open(buf, O_WRONLY);
    if (fd < 0){
        perror("gpio/value");
    }
    
    write(fd, "0", 2);

    close(fd);
}

void Driver::wake(){
    this->asleep = false;
    // Set GPIO Value
    
    int fd;
    char buf[50];
    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio117/direction");

    fd = open(buf, O_WRONLY);
    if (fd < 0){
        perror("gpio/direction");
    }
    
    write(fd, "out", 4);
    
    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio117/value");

    fd = open(buf, O_WRONLY);
    if (fd < 0){
        perror("gpio/value");
    }
    
    write(fd, "1", 2);

    close(fd);
}

bool Driver::isAsleep(){
    return this->asleep;
}

Driver::~Driver(){
    // Wait till the Motor completes its rotation after the program has ended
    while(isMotorBusy == true){}

    this->p0.disable();
    this->p1.disable();
    this->p.shutDown();
}
