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
    void inputCheck(const std::string &temp, bool &slash, bool &underScore, bool &dash);
    int gcd(int p, int q) const;

public:
    mixedNumber();
    ~mixedNumber();
    mixedNumber(double value);
    mixedNumber(int whole, int num = 0, int denom = 1);

    mixedNumber(const Fraction &other);
    mixedNumber(const mixedNumber &other);

    mixedNumber toFraction(const double &num);
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
