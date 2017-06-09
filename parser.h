#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <stack>
#include <sstream>
#include <map>
#include <iostream>
#include <vector>


#include "mixednumber.h"

enum parserErrors{SIGN_STACKING, BAD_INPUT, IMPROPER_PARENTHESES, INVALID_CHAR,
                  CONSECUTIVE_NUMBERS};

class Parser {
public:
    Parser();
    ~Parser();
    Parser(const Parser &other);
    Parser &operator=(const Parser &other);

    Parser &operator<<(const std::string &in);
    Parser &operator>>(std::string &out);

private:
    std::string input, output;
    std::stack<char> operatorStack;
    std::map<char, int> precedence;
    std::vector<mixedNumber> tokens;
    mixedNumber answer;

    void copy(const Parser &other);
    void cleanUp();
    void convert();
    void extract();
    void emptyOperatorStack();

};

#endif // PARSER_H
