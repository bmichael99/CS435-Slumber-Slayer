#include "mbed.h"


// Define pins for the matrix keypad
#define ROW_COUNT 4
#define COL_COUNT 3


DigitalInOut row_pins[ROW_COUNT] = {D8, D7, D6,D5};
DigitalInOut col_pins[COL_COUNT] = {D4,D3,D2};


// Define the keys
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
        
        // Set all row pins as inputs with pull-up
        for (int i = 0; i < ROW_COUNT; i++) {
            row_pins[i].input();
            row_pins[i].mode(PullUp);
        }
        
        // Set all column pins as outputs and high
        for (int j = 0; j < COL_COUNT; j++) {
            col_pins[j].output();
            col_pins[j].write(1);
        }
        
        // Check each column pin
        for (int col = 0; col < COL_COUNT; col++) {
            col_pins[col].write(0); // Set the current column pin low
            
            // Check each row pin
            for (int row = 0; row < ROW_COUNT; row++) {
                if (!row_pins[row].read()) { // Check if the row pin is low
                    pressedKey = keys[row][col];
                    break;
                }
            }
            
            col_pins[col].write(1); // Restore the current column pin to high
            if (pressedKey != '\0') break; // Exit the loop if a key is pressed
        }
        
        return pressedKey;
    }
};


int main() {
    MatrixKeypad keypad;
    printf("Matrix Keypad Test\n");
    
    while (1) {
        char key = keypad.readKey();
        if (key != '\0') {
            printf("Pressed key: %c\n", key);
            ThisThread::sleep_for(200); 
        }
    }
}


