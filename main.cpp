//Michael Brandmeyer
#include "mbed.h"
#include "platform/mbed_thread.h"
#include <string>
#include "NHD_0216HZ.h"
#include "alarm_clock.h"
#include "generate_math.h"
#include "music_player.cpp"

using namespace std;

//define pins for the matrix keypad
#define ROW_COUNT 4
#define COL_COUNT 3


DigitalInOut row_pins[ROW_COUNT] = {D8, D7, D6,D5};
DigitalInOut col_pins[COL_COUNT] = {D4,D3,D2};


//define the keys
char keys[ROW_COUNT][COL_COUNT] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};


class MatrixKeypad {
public:
    MatrixKeypad() {}


    char readKey() {
        char pressedKey = '\0';
        
        //set all row pins as inputs with pull-up
        for (int i = 0; i < ROW_COUNT; i++) {
            row_pins[i].input();
            row_pins[i].mode(PullUp);
        }
        
        //set all column pins as outputs and high
        for (int j = 0; j < COL_COUNT; j++) {
            col_pins[j].output();
            col_pins[j].write(1);
        }
        
        //check each column pin
        for (int col = 0; col < COL_COUNT; col++) {
            col_pins[col].write(0); //set the current column pin low
            
            //check each row pin
            for (int row = 0; row < ROW_COUNT; row++) {
                if (!row_pins[row].read()) { //check if the row pin is low
                    pressedKey = keys[row][col];
                    break;
                }
            }

            col_pins[col].write(1); //restore the current column pin to high
            if (pressedKey != '\0') break; //exit the loop if a key is pressed
        }
        
        return pressedKey;
    }
};

MatrixKeypad keypad;
alarm_clock myClock(true); //declare our clock object
alarm_clock alarmTimeTracker(false);

MusicPlayer player(PC_7); //declare music player
InterruptIn MotionSensor(PC_0); //Motion Sensor

void set_time();
void set_alarm();
void check_alarm();
void clearlcd();
bool answerMath();
void cooldown();
void Moving();
void notMoving();

Ticker checkAlarm_t; //ticker to check if alarm == current time
Ticker alarmCooldown; //ticker for putting checkAlarm_t on a 60 second cooldown after alarm is finished
Timer motionTime; //time since last time 

//values changed inside of different ISR's
volatile bool alarm = false; //check_alarm()
volatile bool moving; //Moving() and notMoving()
volatile float totalTimeMoving;

int main() {
    init_spi();
    init_lcd();
    init_lcd();
    
    set_time(); //set current time
    set_alarm(); //set time for alarm
   
    MotionSensor.rise(&Moving); //if we detect movement call Moving and set moving to true
    MotionSensor.fall(&notMoving); //if we don't detect movement call notMoving and set moving to false

    checkAlarm_t.attach(&check_alarm, 1); //check if alarm == current time
    clearlcd();
    while(1){
        
        clearlcd();
        myClock.displayTime(); //display time

        if(alarm == true){
            motionTime.start(); //motion sensor timer
            player.startMusic();//start alarm sound
            clearlcd();
            set_cursor(0, 1);
            print_lcd("TIME TO WAKE UP!");
            ThisThread::sleep_for(2000);
            while(1){
                if(moving == true){
                    if(answerMath())  {         
                        motionTime.stop();
                        totalTimeMoving += motionTime.read();
                        motionTime.start();   
                        printf("moving & correct answer");
                        if(totalTimeMoving >= 20){ //final condition to end alarm
                            player.stopMusic();
                            alarm = false;
                            checkAlarm_t.detach(); //disable alarm checker
                            alarmCooldown.attach(&cooldown, 60); //re-enable alarm checker after 60 seconds
                            totalTimeMoving = 0;
                            clearlcd();
                            motionTime.stop();
                            break;
                        }
                    }
                } else {
                    motionTime.reset(); //set current time moving to 0
                    totalTimeMoving = 0; //set total moving time to 0 if we detect no movement
                }
                ThisThread::sleep_for(200);
            }
        }
        ThisThread::sleep_for(200); //sleep for 200ms
     }

    
    


}

void set_time(){
    printf("set time");
    clearlcd();
    set_cursor(0, 0);
    print_lcd("Set time");
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
                //Check if we are done setting the time
                if(i >= lcd_time.length()){
                    int hour = stoi(lcd_time.substr(0, 2));
                    int minute = stoi(lcd_time.substr(3, 2));
                    int second = stoi(lcd_time.substr(6, 2));
                    myClock.setTime(hour,minute,second);
                    break;
                }
            } else if(key == '*') { //backspace
                if(i > 0){
                    i -= 1;
                }
                if(lcd_time[i] == ':'){
                    i-=1;
                }
            } else if(i < lcd_time.length()){
                lcd_time[i] = key;
                set_cursor(0, 1);
                print_lcd(&lcd_time[0]);
                i+=1; //don't forget u changed this, delete comment later
            }
            if(lcd_time[i] == ':'){
                    i+=1;    
            }   
            set_cursor(i,1);
            printf("Pressed key: %c\n", key);
            ThisThread::sleep_for(500); 
        }

    }
}

void set_alarm(){
      printf("set alarm");
    clearlcd();
    set_cursor(0, 0);
    print_lcd("Set alarm");
    set_cursor(0, 1);
    print_lcd("00:00:00");
    set_cursor(0, 1);
    set_cursor(0, 1);
    int i = 0;
    string lcd_time = "00:00:00";
    while(1){
        char key = keypad.readKey();
        if (key != '\0') {
            if(key == '#'){ //Enter key
                i+=1;
                //Check if we are done setting the time
                if(i >= lcd_time.length()){
                    int hour = stoi(lcd_time.substr(0, 2));
                    int minute = stoi(lcd_time.substr(3, 2));
                    int second = stoi(lcd_time.substr(6, 2));
                    alarmTimeTracker.setTime(hour,minute,second);
                    break;
                }
            } else if(key == '*') { //backspace
                if(i > 0){
                    i -= 1;
                }
                if(lcd_time[i] == ':'){
                    i-=1;
                }
            } else if(i < lcd_time.length()){
                lcd_time[i] = key;
                set_cursor(0, 1);
                print_lcd(&lcd_time[0]);
                i+=1;
            }

            if(lcd_time[i] == ':'){
                    i+=1;
                    
            }
            
            set_cursor(i,1);

            printf("Pressed key: %c\n", key);
            ThisThread::sleep_for(500); 
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

void Moving(){
    moving = true;
}

void notMoving(){
    moving = false;
}

bool answerMath(){
    printf("Answer math");
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
            printf("Pressed key: %c\n", key);
            if(key == '#'){ //submit answer key
                break;
            } else if(key == '*') { //backspace
                    if(i > 0){
                        mathAnswer = "";
                        i = 0;
                        set_cursor(0,1);
                        print_lcd("        ");
                    }  
            } else { //typing answer
                mathAnswer += key;
                set_cursor(0,1);
                print_lcd(&mathAnswer[0]);
                i+=1;
                
            }
            set_cursor(i,1);
            ThisThread::sleep_for(500);
        }
         
    }

    if(math.checkAnswer(stoi(mathAnswer))){
        clearlcd();
        set_cursor(0, 0);
        print_lcd("Correct!");
        ThisThread::sleep_for(2000);
        set_cursor(0, 0);
        print_lcd("Keep moving to");
        set_cursor(0, 1);
        print_lcd("turn off alarm");
        ThisThread::sleep_for(2000);
        clearlcd();
        return true;
    } else{
        clearlcd();
        set_cursor(0, 0);
        print_lcd("WRONG ANSWER!");
        ThisThread::sleep_for(2000);
        motionTime.reset();
        totalTimeMoving = 0; //maybe turn this off
        return false;
    }
        
}

void clearlcd(){
    set_cursor(0, 0);
    print_lcd("                                                              ");
}

void cooldown(){
    checkAlarm_t.attach(&check_alarm, 1);
}