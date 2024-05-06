//Michael Brandmeyer
#include "mbed.h"
#include "platform/mbed_thread.h"
#include <string>
#include "NHD_0216HZ.h"
#include "alarm_clock.h"
#include "generate_math.h"
#include "keypad.cpp"
#include "music_player.cpp"

using namespace std;

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/
MatrixKeypad keypad;
alarm_clock myClock(true); //declare our clock object
alarm_clock alarmTimeTracker(false);
MusicPlayer player(PA_8); //declare music player

void set_time();
void set_alarm();
void check_alarm();
void checkMotion();
void clearlcd();
bool answerMath();
void cooldown();

Ticker checkAlarm_t; //ticker to check if alarm == current time
Ticker alarmCooldown; //ticker for putting checkAlarm_t on a 60 second cooldown after alarm is finished

volatile bool alarm = false;

//Motion Sensor
InterruptIn MotionSensor(PA_9);

Timer motionTime;
Timer totalTime;
volatile bool moving;
volatile float totalTimeMoving;

int main() {
    
    
    
    
    init_spi();
    init_lcd();
    init_lcd();
    
    
    set_time(); //set current time
    set_alarm(); //set time for alarm

    motionTime.start(); //motion sensor timer
    MotionSensor.rise(&checkMotion); //if we detect movement call checkMotion

    checkAlarm_t.attach(&check_alarm, 1); //check if alarm == current time
    

    while(1){
        
        
        myClock.displayTime(); //display time
        


        if(alarm == true){
            player.startMusic();//start alarm sound
            while(1){
                
                if(moving == true && answerMath()){
                    if(totalTimeMoving >= 30){
                        player.stopMusic();
                        alarm = false;
                        checkAlarm_t.detach(); //disable alarm checker
                        alarmCooldown.attach(&cooldown, 60); //re-enable alarm checker after 60 seconds
                        break;
                    }
                }

                
                ThisThread::sleep_for(1000);

                
                
            }
        }
        ThisThread::sleep_for(200); //sleep for 200ms

        
     }

    
    


}

void set_time(){
    clearlcd();
    set_cursor(0, 0);
    print_lcd("Set current time:");
    set_cursor(0, 1);
    print_lcd("00:00:00");
    set_cursor(0, 1);
    int i = 0;
    string lcd_time = "00:00:00";
    while(1){
        
        

        char key = keypad.readKey();
        if (key != '\0') {
            if(key == '#'){ //Enter key
                i+=1;
                set_cursor(i,1);
                //Check if we are done setting the time
                if(i == lcd_time.length()-1){
                    int hour = stoi(lcd_time.substr(0, 2));
                    int minute = stoi(lcd_time.substr(3, 2));
                    int second = stoi(lcd_time.substr(6, 2));
                    alarmTimeTracker.setTime(hour,minute,second);
                    break;
                }
            } else if(key == '*') { //backspace
                if(i >= 0){
                    i -= 1;
                    set_cursor(i,1);
                }
            } else {
                lcd_time[i] = key;
                print_lcd(&key);
                i+=1; //don't forget u changed this, delete comment later
                set_cursor(i,1);
            }

            if(lcd_time[i] == ':'){
                    i+=1;
            }
            
            
            set_cursor(i,1);

            
            printf("Pressed key: %c\n", key);
            ThisThread::sleep_for(200); 

            
        }

    }
}

void set_alarm(){
    clearlcd();
    set_cursor(0, 0);
    print_lcd("Set alarm time:");
    set_cursor(0, 1);
    print_lcd("00:00:00");
    set_cursor(0, 1);
    int i = 0;
    string lcd_time = "00:00:00";

    while(1){
        
        

        char key = keypad.readKey();
        if (key != '\0') {
            if(key == '#'){ //Enter key
                i+=1;
                set_cursor(i,1);
                //Check if we are done setting the time
                if(i == lcd_time.length()-1){
                    int hour = stoi(lcd_time.substr(0, 2));
                    int minute = stoi(lcd_time.substr(3, 2));
                    int second = stoi(lcd_time.substr(6, 2));
                    alarmTimeTracker.setTime(hour,minute,second);
                    break;
                }
            } else if(key == '*') { //backspace
                if(i >= 0){
                    i -= 1;
                    set_cursor(i,1);
                }
            } else {
                lcd_time[i] = key;
                print_lcd(&key);
                i+=1; //don't forget u changed this, delete comment later
                set_cursor(i,1);
            }

            if(lcd_time[i] == ':'){
                    i+=1;
            }
            
            
            set_cursor(i,1);

            
            printf("Pressed key: %c\n", key);
            ThisThread::sleep_for(200); 

            
        }

    }
}

void check_alarm(){
    
    if(alarm != true && 
    myClock.getHour() == alarmTimeTracker.getHour() && 
    myClock.getMinute() == alarmTimeTracker.getMinute()){
        alarm = true;
        checkAlarm_t.detach();
        
    }
    
}

void checkMotion(){
    motionTime.stop();
    
    if(motionTime.read() > 5){
        moving = false;
    } else {
        moving = true;
        totalTimeMoving += motionTime.read();
    }
    motionTime.start();
}

bool answerMath(){
    clearlcd();
    set_cursor(0, 0);
    generate_math math;
    string mathProblem = math.getEquation();
    print_lcd(&mathProblem[0]);
    set_cursor(0, 1);

    int i = 0;
    string mathAnswer = "";

    while(1){
        char key = keypad.readKey();
        if (key != '\0') {
            if(key == '#'){ //submit answer key
                break;
            } else if(key == '*') { //backspace
                if(i >= 0){
                    i -= 1;
                    set_cursor(i,1);
                    mathAnswer.erase(i,1);
                    print_lcd(" ");
                }
            } else { //typing answer
                mathAnswer[i] = key;
                print_lcd(&key);
                i+=1;
                set_cursor(i,1);
            }
        }

    }

    if(math.checkAnswer(stoi(mathAnswer))){
        clearlcd();
        set_cursor(0, 0);
        print_lcd("Correct!");
        ThisThread::sleep_for(1500);
        set_cursor(0, 0);
        print_lcd("Keep moving to");
        set_cursor(0, 1);
        print_lcd("turn off the alarm");
        ThisThread::sleep_for(1000);
        return true;
    } else{
        clearlcd();
        set_cursor(0, 0);
        print_lcd("WRONG ANSWER!");
        totalTimeMoving = 0; //maybe turn this off
        return false;
    }
        
}

void clearlcd(){
    set_cursor(0, 0);
    print_lcd("                                                ");
}

void cooldown(){
    checkAlarm_t.attach(&check_alarm, 1);
}