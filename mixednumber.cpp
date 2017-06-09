#include "mixednumber.h"

/// CONSTRUCTORS
///
mixedNumber::mixedNumber() {}

mixedNumber::~mixedNumber() {}

mixedNumber::mixedNumber(double value) {
    Fraction temp(toFraction(value));
    set(temp.getNum(), temp.getDenom());
}

mixedNumber::mixedNumber(int whole, int num, int denom) {
    int neg = whole < 0 ? -1 : 1;
    set(neg * (denom * (whole * neg) + num), denom);
}

mixedNumber::mixedNumber(const mixedNumber &other) {
    copy(other);
}

mixedNumber::mixedNumber(const Fraction &other) {
    copy(other);
}


/// PRIVATE FUNCTIONS
///
void mixedNumber::copy(const Fraction &other) {
    set(other.getNum(), other.getDenom());
}

void mixedNumber::copy(const mixedNumber &other) {
    set(other.getNum(),other.getDenom());
}

int mixedNumber::gcd(int p, int q) const {
    return q == 0 ? p : gcd(q, p % q);
}

/// PUBLIC FUNCTIONS
///

mixedNumber mixedNumber::toFraction(const double &x) {
    if(x == 0) {
        return mixedNumber(0, 1);
    }
    else {
        const long PERCISION = 1000000000;
        double whole = std::floor(x);
        double frac = x - whole;
        int gcdenom = gcd(round(frac * PERCISION), PERCISION);
        long den = PERCISION / gcdenom;
        long num = round(frac * PERCISION) / gcdenom;
        return Fraction((num+(den*whole)), den);
    }
}

int mixedNumber::getWhole() const {
    return static_cast<int>(getNum() / getDenom());
}

Fraction mixedNumber::getFrac() const {
    return Fraction(getNum(), getDenom());
}

/// COPY
///
mixedNumber &mixedNumber::operator=(const mixedNumber &other) {
    if(this != &other)
        copy(other);
    return *this;
}

mixedNumber &mixedNumber::operator=(const Fraction &other) {
    if(this != &other)
        copy(other);
    return *this;
}

/// I/O OPERATORS
///
std::ostream &operator<<(std::ostream &out, const mixedNumber &x) {

    // Gets fraction data and retrieves whole number
    int num = x.getNum(),
        denom = x.getDenom(),
        whole = num/denom;

    // Reduces the fraction to a mixed Fraction style
    num %= denom;

    // Prints fraction and adds whole number to front
    if(whole != 0) {

        // Ensures numerator isn't negative
        if(num < 0)
            num *= -1;

        // Prints in format : 3_4/5
        out << whole;
        if(num != 0)
            out << ' ' << Fraction(num,denom);
    }
    else
        out << Fraction(num,denom);

    return out;
}

std::istream &operator>>(std::istream &in, mixedNumber &x) {

    // Throws away leading whitespaces
    char ch;
    do { in.get(ch); } while(ch == ' ');

    // Converts user input into string
    std::stringstream toString;
    std::string temp;
    do {
        toString << ch;
        in.get(ch);
    } while (ch != '\n' && !in.eof());

    getline(toString, temp);

    // Throws error if input is too long
    if(&in == &std::cin && temp.length() > 15) throw INPUT_TOO_LONG;

    // Checks input for all possible errors
    bool slash = false, underScore = false, dash = false;
    x.inputCheck(temp, slash, underScore, dash);

    /// CONVERSIONS - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Puts string into mixed number
    if(underScore && slash) {

        size_t uSLoc = temp.find(' ');
        size_t slLoc = temp.find('/');

        // Builds the whole part
        std::string whole = temp.substr(0, uSLoc);
        if(dash) whole = whole.substr(1);

        // Builds fraction part
        std::string num = temp.substr(uSLoc + 1, (slLoc - uSLoc - 1));
        std::string denom = temp.substr(slLoc + 1);

        // Puts both parts together and makes negative if needed
        x.set(atoi(denom.c_str()) * atoi(whole.c_str()) + atoi(num.c_str()), atoi(denom.c_str()));
        if(dash) x *= -1;
    }

    // Puts string into improper fraction
    else if(slash) {

        size_t slLoc = temp.find('/');

        // Builds fraction part
        std::string insert = temp.substr(0, slLoc);
        std::string insertTwo = temp.substr(slLoc + 1);

        x.set(atoi(insert.c_str()), atoi(insertTwo.c_str()));
    }
    // Sets only whole number
    else {
        x.set(atoi(temp.c_str()), 1);
    }
    return in;
}

/// @brief Checks input for errors and tracks existance of key characters
///
void mixedNumber::inputCheck(const std::string &temp, bool &slash, bool &underScore, bool &dash) {

    // Scans for all the edge cases and throws errors
    for(uint i = 0; i < temp.length(); ++i) {

        // throw if invalid character
        if(!isdigit(temp[i]) && temp[i] != ' ' && temp[i] != '/' && temp[i] != '-')
            throw HAS_ALPHABET;

        // throw if more than one slash
        if(temp[i] == '/' && (slash))
            throw IMPROPER_SYMBOL_USE;
        if(temp[i] == '/' && !slash)
            slash = true;

        // throw if more than one underscore
        if(temp[i] == ' ' && (slash || underScore))
            throw IMPROPER_SYMBOL_USE;
        if(temp[i] == ' ' && !underScore) {
            // Ensures there is a number before '_'
            if(i == 0)
                throw IMPROPER_SYMBOL_USE;
            underScore = true;
        }

        // throw if more than one dash
        if(temp[i] == '-' && (dash || slash || underScore))
            throw IMPROPER_SYMBOL_USE;
        if(temp[i] == '-' && !dash) {
            // Ensures - is the first character
            if(i != 0)
                throw IMPROPER_SYMBOL_USE;
            dash = true;
        }
    }

    // Ensures underscore is paired with slash
    if(underScore && !slash)
        throw IMPROPER_SYMBOL_USE;

    // Ensures something between underscore and slash
    if(underScore) {
        if(temp.find('/') - temp.find(' ') == 1)
            throw IMPROPER_SYMBOL_USE;
    }
    // Ensures final character is a digit
    if(!isdigit(temp[temp.length() - 1]))
        throw IMPROPER_SYMBOL_USE;
}
