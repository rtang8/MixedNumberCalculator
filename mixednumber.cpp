#include "mixednumber.h"

/// CONSTRUCTORS
///
mixedNumber::mixedNumber() {
}

mixedNumber::~mixedNumber() {}

mixedNumber::mixedNumber(int whole, int num, int denom) {
    int neg = whole < 0 ? -1 : 1;
    set(neg * (denom * (whole * neg) + num), denom);
}


void mixedNumber::copy(const Fraction &other) {
    //cout<<"Firing mixedNumber\'s fraction copy constructor"<<endl;
    set(other.getNum(), other.getDenom());
}

void mixedNumber::copy(const mixedNumber &other) {
    //cout<<"Firing mixedNumber\'s mixedNumber copy constructor"<<endl;
    set(other.getNum(),other.getDenom());
}

mixedNumber::mixedNumber(const mixedNumber &other) {
    copy(other);
}

mixedNumber::mixedNumber(const Fraction &other) {
    copy(other);
}

/// PRIVATE FUNCTIONS
///
//void mixedNumber::copy(const Fraction &other) {
//    m_whole = other.m_whole;
//    m_frac = other.m_frac;
//}

/// PUBLIC FUNCTIONS
///

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
            out << '_' << Fraction(num,denom);
    }
    else
        out << Fraction(num,denom);

    return out;
}

/*std::istream &operator>>(std::istream &in, mixedNumber &x) {

    char ch;

    // Throws away leading whitespaces
    do {
        in.get(ch);
    } while(ch == ' ');

    // Booleans track existance of characters
    bool slash = false;
    bool underScore = false;
    bool dot = false;
    bool dash = false;
    std::string temp;
    std::stringstream toString;

    // Converts user input into string
    do {
          toString << ch;
          in.get(ch);
    } while (ch != '\n' && !in.eof());

    toString >> temp;

    // Throws error if input is too long
    if(&in == &std::cin && temp.length() > 15) {
        throw INPUT_TOO_LONG;
    }

    /// INPUT CHECKING - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Scans for all the edge cases and throws errors
    for(uint i = 0; i < temp.length(); ++i) {

        // throw if invalid character
        if(!isdigit(temp[i]) && temp[i] != '.' && temp[i] != '_' && temp[i] != '/' && temp[i] != '-')
            throw HAS_ALPHABET;
        // throw if more than one dot
        if(temp[i] == '.' && (dot || slash || underScore))
            throw IMPROPER_SYMBOL_USE;
        if(temp[i] == '.' && !dot)
            dot = true;

        // throw if more than one slash
        if(temp[i] == '/' && (dot || slash))
            throw IMPROPER_SYMBOL_USE;
        if(temp[i] == '/' && !slash)
            slash = true;

        // throw if more than one underscore
        if(temp[i] == '_' && (dot || slash || underScore))
            throw IMPROPER_SYMBOL_USE;
        if(temp[i] == '_' && !underScore) {
            // Ensures there is a number before '_'
            if(i == 0)
                throw IMPROPER_SYMBOL_USE;
            underScore = true;
        }

        // throw if more than one dash
        if(temp[i] == '-' && (dash || dot || slash || underScore))
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
        if(temp.find('/') - temp.find('_') == 1)
            throw IMPROPER_SYMBOL_USE;
    }
    // Ensures final character is a digit
    if(!isdigit(temp[temp.length() - 1]))
        throw IMPROPER_SYMBOL_USE;


    /// CONVERSIONS - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Puts string into double
    if(dot) {
        double value;
        std::stringstream ss;
        ss << temp;
        ss >> value;
        x = Fraction(value);
        return in;
    }

    // Puts string into mixed number
    else if(underScore && slash) {

        size_t uSLoc = temp.find('_');
        size_t slLoc = temp.find('/');

        // Builds the whole part
        std::string insert = temp.substr(0, uSLoc);
        if(dash)
            insert = insert.substr(1);
        mixedNumber tempMix(atoi(insert.c_str()));

        // Builds fraction part
        insert = temp.substr(uSLoc + 1, (slLoc - uSLoc - 1));

        std::string insertTwo = temp.substr(slLoc + 1);
        Fraction part(atoi(insert.c_str()), atoi(insertTwo.c_str()));

        // Adds the two parts together
        x = tempMix + part;
        if(dash)
            x *= -1;
    }

    // Puts string into improper fraction
    else if(slash) {

        size_t slLoc = temp.find('/');

        // Builds fraction part
        std::string insert = temp.substr(0, slLoc);
        std::string insertTwo = temp.substr(slLoc + 1);

        mixedNumber finalIn(0, atoi(insert.c_str()), atoi(insertTwo.c_str()));
        x = finalIn;
    }
    else {
        x = mixedNumber(atoi(temp.c_str()));
    }
    return in;
}*/
