#include "mbed.h"
#include <string>

using namespace std;

#ifndef ALARM_CLOCK_H
#define ALARM_CLOCK_H

class alarm_clock {

private:          
    volatile int second;  
    volatile int minute;  
    volatile int hour;
    Ticker countTime;

public:
    alarm_clock(int second, int minute, int hour); // Constructor declaration
    void updateTime();
    string getSecond();
    string getMinute();
    string getHour();
    void displayTime();
};


#endif //ALARM_CLOCK_H
