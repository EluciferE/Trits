#ifndef TRITS_TRITSET_H
#define TRITS_TRITSET_H

#include <iostream>
#include <cmath>
#include <cstring>

#include "trit.h"

using namespace std;


#define uint unsigned int

enum CountType {add, del};


class TritSet {
private:
    size_t num_size;
    size_t mem_size;
    uint *set;

    size_t num_false;
    size_t num_true;
    size_t last_trit;

    static size_t set_size(size_t size);
    static uint trit_value(Trit trit, size_t index);
    void count_trit(Trit trit, CountType type_);
    void equal_sizes(TritSet& set1);
    void extend_size(size_t size);

    class Proxy {
    private:
        TritSet *tritSet;
        size_t index;

    public:
        explicit Proxy(TritSet *trit_set, size_t ind);
        void operator=(const Trit trit);
        operator const Trit();
    };

public:
    class Iterator{

    private:
        TritSet* tritSet;
        size_t index_now;

    public:
        explicit Iterator(TritSet* trit_set, size_t index);
        Iterator operator++ ();
        Iterator operator-- ();
        bool operator==(const Iterator& iterator) const;
        bool operator!=(const Iterator& iterator) const;

        Proxy operator*();
    };
    Iterator begin();
    Iterator end();

    Proxy operator[](size_t index);
    TritSet operator|(TritSet& set1);
    TritSet operator&(TritSet& set1);
    TritSet operator~();

    size_t capacity();
    size_t cardinality(Trit value);
    explicit TritSet(size_t size);


};


#endif //TRITS_TRITSET_H
