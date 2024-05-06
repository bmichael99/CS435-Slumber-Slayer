//Abir
#include "generate_math.h"
#include <cstdlib>
#include <ctime>
#include <iostream>


using namespace std;



// Constructor
generate_math::generate_math() {
    // Initialize random seed
    srand(time(0));
    generateNewEquation();
}

// Generate a new equation
void generate_math::generateNewEquation() {
    operand1 = rand() % 10; // Numbers between 0 and 9
    operand2 = rand() % 10;
    int op = rand() % 3; // Choose an operation: 0, 1, or 2
    switch (op) {
        case 0:
            operation = '+';
            break;
        case 1:
            operation = '-';
            break;
        case 2:
            operation = '*';
            break;
    }
    calculateResult();
}

// Get the equation as a string
string generate_math::getEquation() const {
    return to_string(operand1) + " " + operation + " " + to_string(operand2) + " = ?";
}

// Check the answer
bool generate_math::checkAnswer(int userAnswer) const {
    return userAnswer == result;
}

// Get the correct answer (for display purposes)
int generate_math::getCorrectAnswer() const {
    return result;
}

// Helper function to calculate the result
void generate_math::calculateResult() {
    switch (operation) {
        case '+':
            result = operand1 + operand2;
            break;
        case '-':
            result = operand1 - operand2;
            break;
        case '*':
            result = operand1 * operand2;
            break;
        default:
            cout << "Invalid operation" << endl;
    }
}