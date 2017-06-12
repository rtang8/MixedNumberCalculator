#include <iostream>

#include "fraction.h"
#include "mixednumber.h"
#include "parser.h"

using namespace std;

/// Prototypes
bool readInput(string &line);
void process(const string &input, string &output, mixedNumber &answer);

/// MAIN - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
///
int main() {
    string input, output;
    mixedNumber finalAnswer;
    while(readInput(input)) {
        try {
            process(input, output, finalAnswer);
            cout << "RPN Conversion: " << output << endl;
            cout << "Answer: " << finalAnswer << endl;
        }
        catch (parserErrors e) {
            if(e == SIGN_STACKING)
                cout << "ERROR : Can only have one operator between numbers." << endl;
            if(e == BAD_INPUT)
                cout << "ERROR : Bad Input!" << endl;
            if(e == IMPROPER_PARENTHESES)
                cout << "ERROR : Improper Parentheses Usage." << endl;
            if(e == INVALID_CHAR)
                cout << "ERROR : Entered an invalid character." << endl;
            if(e == CONSECUTIVE_NUMBERS)
                cout << "ERROR : Must have operator between numbers." << endl;
            if(e == CONSECUTIVE_DECIMALS)
                cout << "ERROR : Cannot have multiple decimals in a number." << endl;
            if(e == DIVIDE_BY_ZERO)
                cout << "ERROR : Cannot divide by zero." << endl;
        }
        catch (...) {
            cout << "Unknown Error Occured." << endl;
        }
    }
    return 0;
}

/// @brief Reads line from user, and ends program is empty line entered
///
bool readInput(string &line) {
    cout << "Input: ";
    getline(cin, line);
    return !line.empty();
}

/// @brief Converts string to rpn and solves it
void process(const string &input, string &output, mixedNumber &answer) {
    static Parser converter;
    converter << input;
    converter >> output;
    answer = converter.getAnswer();
}
