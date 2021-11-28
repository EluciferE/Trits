#include <iostream>
#include "tritset.h"

int main() {
    TritSet setA(100);
    setA[0] = True;
    TritSet setB(setA);
    setA[10] = setB[150] = True;
    cout << setB[0];
    return 0;
}
