#include <iostream>

#include "fraction.h"
#include "mixednumber.h"

using namespace std;

int main(int argc, char *argv[]) {
    mixedNumber test(-1, 0, 1);
    mixedNumber test2(1, 0, 1);

    cout << test << " + " << test2 << endl;
    cout << test + test2 << endl;
    return 0;
}
