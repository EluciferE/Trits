#ifndef TRITS_TRITSET_H
#define TRITS_TRITSET_H

#include <iostream>
#include <cmath>
#include <cstring>

#include "trit.h"

using namespace std;


#define uint unsigned int


class TritSet{
private:
    static size_t set_size(size_t size);
    static uint trit_value(Trit trit, size_t index);

    size_t num_size;
    size_t mem_size;
    uint* set;

    size_t num_false;
    size_t num_true;

    class Proxy{
    private:
        TritSet* tritSet;
        size_t index;

        void extend_set(int new_size);

    public:
        explicit Proxy(TritSet* trit_set, size_t ind);
        void operator = (const Trit trit);
        operator const Trit ();
    };

    public:
        Proxy operator[](size_t index);
        size_t capacity();

        explicit TritSet(size_t size);



};






#endif //TRITS_TRITSET_H
