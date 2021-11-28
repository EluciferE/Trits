#include <iostream>
#include "tritset.h"

int main() {
    TritSet set(1000);
    set[9999] = True;
    set[10000] = False;
    std::cout << set[9999] << set[10000];
    return 0;
}
