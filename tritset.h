#ifndef TRITS_TRITSET_H
#define TRITS_TRITSET_H

#include <iostream>
#include <cmath>
#include <cstring>
#include <unordered_map>

#include "trit.h"

using uint = unsigned int;

enum CountType {add, del};


class TritSet {
private:
    static constexpr size_t TRIT_IN_UINT = sizeof(uint) * 4;
    static constexpr size_t BIT_IN_UINT = sizeof(uint) * 8;

    size_t num_size;
    size_t mem_size;
    size_t basic_size;

    size_t num_false;
    size_t num_true;
    size_t logical_length;

    static size_t set_size(const size_t size);
    static uint trit_value(const Trit trit, const size_t index);
    void count_trit(const Trit trit, const CountType type_);
    void equal_sizes(TritSet& set1);
    void extend_size(const size_t size);
    void copy_class(const TritSet& tritSet);

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
    uint *set;
    class Iterator{

    private:
        TritSet* tritSet;
        size_t index_now;

    public:
        explicit Iterator(TritSet* trit_set, size_t index);

        Proxy operator*();

        Iterator operator++ ();
        Iterator operator-- ();

        bool operator==(const Iterator& iterator) const;
        bool operator!=(const Iterator& iterator) const;


    };
    Iterator begin();
    Iterator end();

    Proxy operator[](const size_t index);
    TritSet operator|(TritSet& set1);
    TritSet operator&(TritSet& set1);
    TritSet operator~();
    Trit operator[](const size_t index) const;

    [[nodiscard]] size_t capacity() const;
    [[nodiscard]] size_t cardinality(const Trit value) const;
    [[nodiscard]] size_t length() const;

    std::unordered_map< Trit, int, std::hash<int>> cardinality() const;

    void shrink();
    void trim(size_t lastIndex);
    explicit TritSet(size_t size);
    TritSet(const TritSet& tritSet);
};


#endif //TRITS_TRITSET_H
