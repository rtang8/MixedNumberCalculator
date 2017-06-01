#ifndef MIXEDNUMBER_H
#define MIXEDNUMBER_H

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include "fraction.h"

enum mixedNumberErrors {INPUT_TOO_LONG, BAD_NUM_INPUT, HAS_ALPHABET, IMPROPER_SYMBOL_USE};

class mixedNumber : public Fraction {

private:
    void copy(const mixedNumber& other);
    void copy(const Fraction& other);

public:
    mixedNumber();
    ~mixedNumber();
    mixedNumber(int whole, int num = 0, int denom = 1);

    mixedNumber(const Fraction &other);
    mixedNumber(const mixedNumber &other);

    int getWhole() const;
    Fraction getFrac() const;

    mixedNumber &operator=(const Fraction &other);
    mixedNumber &operator=(const mixedNumber &other);

    friend
    std::ostream &operator<<(std::ostream &out, const mixedNumber &x);
    friend
    std::istream &operator>>(std::istream &in, mixedNumber &x);

};

#endif // MIXEDNUMBER_H
