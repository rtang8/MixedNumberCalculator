#include <iostream>

#include "fraction.h"
#include "mixednumber.h"

using namespace std;

int main(int argc, char *argv[]) {
    mixedNumber test(-2, 4, 5);
    mixedNumber test2(1, 0, 1);

    cin >> test;
    cin >> test2;
    cout << test << " + " << test2 << endl;
    cout << mixedNumber(test + test2) << endl;
    return 0;
}
