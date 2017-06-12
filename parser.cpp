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
    evaluate();
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
    while(!tokens.empty())
        tokens.pop();
    input = output = "";
}

/// Converts to RPN and does complete error checking
///
void Parser::convert() {
    char value;
    bool negative = true,
         frac = true;
    bool open = false,
         sign = false,
         number = false,
         mixed = false,
         decimal = false,
         rBracket = false;

    std::stringstream ss;
    ss << input;

    // Ensures that input starts on number
    while(ss.peek() == ' ') { ss.get(); }
    if(!isdigit(ss.peek()) && ss.peek() != '-' && ss.peek() != '.' && ss.peek() != '(') {
        cleanUp();
        throw BAD_INPUT;
    }

    while(ss >> value) {

        switch(value) {
            case ')' : // ------------------------------------------

                // Ensures there is a number before closing paren.
                if(sign || negative) {
                    cleanUp();
                    throw IMPROPER_PARENTHESES;
                }

                emptyOperatorStack();
                open = decimal = false;
                rBracket = true;
                break;

            case '(' : // ------------------------------------------
                open = true;
                rBracket = decimal = false;
                break;

            case '.' : // ------------------------------------------
                       // Sets "decimal" to true, which ensures only one decimal
                       // Decimal only set to false once an op is hit
                if(decimal) {
                    cleanUp();
                    throw CONSECUTIVE_DECIMALS;
                }
                decimal = true;
                // Allows to inputs that start with ., like .25
                if(output.size() != 0)
                    output.pop_back();
                output += value;
                break;

            case '/' : // ------------------------------------------
                       // Puts fractions and mixed numbers with proper / notation
                       // "mixed" is true when 2 # + space, ex. (1 2)
                       // "frac" is true after any other op besides another /
                       // "rBracket" so (3 + 4)/... isn't interpreted as a frac
                if((mixed || frac) && !(rBracket) &&!(decimal)) {
                    output.pop_back();
                    output += value;
                    // Temporarily appends an underscore for calculatation parsing
                    // 2 2/3 --> 2_2/3 during calc --> 2 2/3 final print
                    if(output.find_last_of(' ') != std::string::npos && mixed)
                        output[output.find_last_of(' ')] = '_';

                    mixed = frac = number = false;

                    // Ensures you can't insert like '3/'
                    while(ss.peek() == ' ') { ss.get(); }
                    if(!isdigit(ss.peek())) {
                        cleanUp();
                        throw BAD_INPUT;
                    }
                    break;
                }


            case '-' :
                if(negative) {
                    // Ensures only one negative applied
                    // As such "--2" is allowed ("minus neg. 2")
                    //but "---2" is not
                    if(output[output.size()-1] == '-') {
                        cleanUp();
                        throw SIGN_STACKING;
                    }
                    output += '-';
                    rBracket = false;
                    break;
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

                negative = frac = true;
                number = rBracket = decimal = false;

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

                // Adds space to string
                if (ss.peek() > '9' || ss.peek() < '0') {
                    output += " ";
                    // Checks if there is a potential for a mixed number
                    if(number && !decimal) {
                        mixed = true;
                    }
                    number = true;
                }

                // Sets tracking bools appropriately
                sign = negative = rBracket = false;
       }
    }

    // Ensures input does not end in operator, like "1 +"
    if(sign) {
        cleanUp();
        throw BAD_INPUT;
    }
    // Ensures input doesn't have incomplete mixed num, ex. "1 2"
    if(mixed) {
        cleanUp();
        throw CONSECUTIVE_NUMBERS;
    }
    // Ensures floats have numbers after decimal
    if(output[output.size() - 1] == '.') {
        cleanUp();
        throw BAD_INPUT;
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

void Parser::evaluate() {

    mixedNumber first, second, convert;
    std::stringstream ss(output);
    std::stringstream converter;
    std::string temp;

    char op;
    clearAccumulator();

    // Cycles through translated line token by token
    while(ss.peek() != EOF) {
        temp.clear();

        // Gets a string block separated spaces (like 1_2/3 or 2/4 or 1.2)
        while(ss.peek() != ' ') { temp += ss.get(); }

        // If string block is not an op, convert to a mixedNum and push to stack
        if(temp != "+" && temp != "-" && temp != "*" && temp != "/" && temp != "") {

            // Replaces underscores in mixed numbers with spaces before convert
            if(temp.find('_') != std::string::npos)
                temp[temp.find('_')] = ' ';

            converter.str(std::string());
            converter.clear();
            converter << temp;
            converter >> convert;
            accumulator.push(convert);
        }

        // If string block is an operator, apply approriate operation
        else {
            op = temp[0];
            switch(op) {
               case '*' : popTwo(first, second);
                          accumulator.push(first * second);
                          break;
               case '/' : popTwo(second, first);
                          if(second == 0)
                              throw DIVIDE_BY_ZERO;
                          accumulator.push(first / second);
                          break;
               case '+' : popTwo(first, second);
                          accumulator.push(first + second);
                          break;
               case '-' : popTwo(second, first);
                          accumulator.push(first - second);
                          break;
            }
        }

        // Removes space after each token
        if(ss.peek() != EOF) {
            ss.get();
        }
    }
    answer = accumulator.top();
}

void Parser::clearAccumulator() {
    while(!accumulator.empty())
        accumulator.pop();
}

void Parser::popTwo(mixedNumber &first, mixedNumber &second) {
   first = accumulator.top();
   accumulator.pop();
   second = accumulator.top();
   accumulator.pop();
}

mixedNumber Parser::getAnswer() const {
    return answer;
}
