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


    void calculateResult();

public:

    generate_math();
    void generateNewEquation();
    string getEquation() const;
    bool checkAnswer(int userAnswer) const;
    int getCorrectAnswer() const;
};

#endif // MATHEQUATION_H