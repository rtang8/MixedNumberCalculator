#include "parser.h"


/// CONSTRUCTORS
///
Parser::Parser() {
    precedence[')'] = 3;
    precedence['*'] = 2;
    precedence['/'] = 2;
    precedence['+'] = 1;
    precedence['-'] = 1;
    precedence['('] = 0;
}

Parser::~Parser() {
    cleanUp();
}

Parser::Parser(const Parser &other) {
    copy(other);
}

Parser &Parser::operator=(const Parser &other) {
    if(this != &other) {
        cleanUp();
        copy(other);
    }
    return *this;
}

/// IO OPERATORS
///
Parser &Parser::operator<<(const std::string &in) {
    input = in;
    convert();
}

Parser &Parser::operator>>(std::string &out) {
    out = output;
    cleanUp();
    return *this;
}

/// PRIVATE FUNCTIONS
///
void Parser::copy(const Parser &other) {
    input = other.input;
    output = other.output;
    operatorStack = other.operatorStack;
}

void Parser::cleanUp() {
    while(!operatorStack.empty())
        operatorStack.pop();
    input = output = "";
    tokens.clear();
}

/// Converts to RPN and does complete  error checking
///
/// FIX MINUS SIGN
/// FIX 3/ BUG
/// FIX DECIMAL POINTS!!!
///
void Parser::convert() {
    char value;
    bool open = false;
    bool negative = true;
    bool sign = false;
    bool number = false;
    bool mixed = false;
    bool frac = true;

    int inputLen = 0;

    std::stringstream ss;
    ss << input;

    // Ensures that input starts on number
    if(!isdigit(ss.peek()) && ss.peek() != '-' && ss.peek() != '.') { throw BAD_INPUT;}

    while(ss >> value) {
        switch(value) {
            case ')' : // ------------------------------------------

                // Ensures there is a number before closing paren.
                if(sign || negative) {
                    cleanUp();
                    throw IMPROPER_PARENTHESES;
                }

                emptyOperatorStack();
                open = false;
                break;

            case '(' : // ------------------------------------------
                open = true;
                break;

            case '-' : // ------------------------------------------
                if(negative) {
                    // Ensures only one negative applied
                    if(output[output.size()-1] == '-') {
                        cleanUp();
                        throw SIGN_STACKING;
                    }
                    output += '-';
                    break;
                }
            case '/' :
                // Puts fractions and mixed numbers with proper / notation
                if(mixed || frac) {
                    mixed = false;
                    frac = false;
                    number = false;
                    output.pop_back();
                    output += value;
                    break;
                    if(!isdigit(ss.peek())) {
                        cleanUp();
                        throw BAD_INPUT;
                    }
                }
            case '*' :
            case '+' :

                // Ensures only one sign between numbers
                if(sign) {
                    cleanUp();
                    throw SIGN_STACKING;
                }

                sign = true;

                if(!operatorStack.empty() && !open &&
                  (precedence[value] < precedence[operatorStack.top()])) {
                    extract();
                    operatorStack.push(value);
                }
                else {
                    operatorStack.push(value);
                    open = false;
                }

                // Next '-' after a sign is a negative sign
                negative = true;
                number = false;
                frac = true;
                break;

            default :  // ------------------------------------------

                // Checks if character is valid
                if(!(value <= '9' && value >= '0')
                        && (precedence.find(value) == precedence.end())
                        && value != '.') {
                    cleanUp();
                    throw INVALID_CHAR;
                }

                output += value;
                ++inputLen;

                // Adds space to string
                if(ss.peek() > '9' || ss.peek() < '0') {
                    output += " ";
                    // Checks if there is a potential for a mixed number
                    if(number) {
                        std::cout << "DEBUG: Possible Mixed Number Detected" << std::endl;
                        mixed = true;
                    }
                    number = true;
                }

                // Sets tracking bools appropriately
                sign = false;
                negative = false;
       }
    }

    // Ensures input does not end in operator
    if(sign) {
        cleanUp();
        throw BAD_INPUT;
    }
    if(mixed) {
        cleanUp();
        throw CONSECUTIVE_NUMBERS;
    }
    emptyOperatorStack();
}

void Parser::extract() {
    output += operatorStack.top();
    output += ' ';
    operatorStack.pop();
}

void Parser::emptyOperatorStack() {
    while(!operatorStack.empty())
        extract();
}
