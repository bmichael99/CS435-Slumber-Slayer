//Jonathan
#include "mbed.h"


// Define pins for the matrix keypad
#define ROW 4
#define COL 3


DigitalInOut row_pins[ROW] = {D8, D7, D6,D5};
DigitalInOut col_pins[COL] = {D4,D3,D2};


// matrix lay out
char keys[ROW][COL] = {
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
        
        // set all row pins as inputs with pull-up (set to 1)
        for (int i = 0; i < ROW; i++) {
            row_pins[i].input();
            row_pins[i].mode(PullUp);
        }
        
        // set all column pins as outputs and high
        for (int j = 0; j < COL; j++) {
            col_pins[j].output();
            col_pins[j].write(1);
        }
        
        // check each column pin
        for (int col = 0; col < COL; col++) {
            col_pins[col].write(0); // Set the current column pin low
            
            // check each row pin
            for (int row = 0; row < ROW; row++) {
                if (!row_pins[row].read()) { // Check if the row pin is low THIS IS THE KEY THAT IS PRESSED
                    pressedKey = keys[row][col];
                    break;
                }
            }
            
            col_pins[col].write(1); // restore the current column pin to high
            if (pressedKey != '\0') break; // exit the loop if a key is pressed
        }
        
        return pressedKey;
    }
};



