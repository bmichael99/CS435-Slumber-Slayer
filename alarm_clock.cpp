#include "mbed.h"
#include "alarm_clock.h"
#include "NHD_0216HZ.h"
#include <string>



alarm_clock::alarm_clock(int hour,int minute, int second){
    countTime.attach(callback(this, &alarm_clock::updateTime), 1);

    //exception handling for invalid time
    if(second > 59){
        minute += (int)second/60;
        second = second % 60;
    }
    if(minute > 59){
        hour += (int)minute/60;
        minute = minute % 60;
    }
    if(hour > 23){
        hour = hour % 24;
    }
    
    this->second = second;
    this->minute = minute;
    this->hour = hour;
}


void alarm_clock::updateTime(){
    this->second +=1;
    if(this->second > 59){
        this->second = 0;
        this->minute+=1;
    }
    if(this->minute > 59){
        this->minute = 0;
        this->hour+=1;
    }
    if(this->hour > 23){
        this->hour = 0;
    }
}


string alarm_clock::getSecond(){
    string str_second = to_string(this->second);
    return str_second;
}
string alarm_clock::getMinute(){
    string str_minute = to_string(this->minute);
    return str_minute;
}
string alarm_clock::getHour(){
    string str_hour = to_string(this->hour);
    return str_hour;
}

void alarm_clock::displayTime(){



    uint32_t old_primask = __get_PRIMASK();
    __disable_irq();
    string str_hour = getHour();
    string str_minute = getMinute();
    string str_second = getSecond();
    __set_PRIMASK(old_primask);

    if(str_hour.length() < 2){
            str_hour = '0' + str_hour;
    }
    if(str_minute.length() < 2){
            str_minute = '0' + str_minute;
    }
    if(str_second.length() < 2){
            str_second = '0' + str_second;
    }

    set_cursor(0, 0);
    print_lcd(&str_hour[0]);
    set_cursor(2, 0);
    print_lcd(":");
    set_cursor(3, 0);
    print_lcd(&str_minute[0]);
    set_cursor(5, 0);
    print_lcd(":");
    set_cursor(6, 0);
    print_lcd(&str_second[0]);
    set_cursor(8, 0);
}