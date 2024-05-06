#ifndef MATHEQUATION_H
#define MATHEQUATION_H

#include <string>

using namespace std;

class generate_math {
private:
    int operand1;
    int operand2;
    char operation;
    int result;

    // Helper function to calculate the result
    void calculateResult();

public:
    // Constructor
    generate_math();

    // Generate a new equation
    void generateNewEquation();

    // Get the equation as a string
    string getEquation() const;

    // Check the answer
    bool checkAnswer(int userAnswer) const;

    // Get the correct answer (for display purposes)
    int getCorrectAnswer() const;
};

#endif // MATHEQUATION_H