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
    alarm_clock(int second, int minute, int hour, bool update); // Constructor declaration
    alarm_clock(bool update);
    void updateTime();
    string getSecond();
    string getMinute();
    string getHour();
    void displayTime();
    void setTime(int hour,int minute, int second);
};


#endif //ALARM_CLOCK_H
