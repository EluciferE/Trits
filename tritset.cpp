#include "tritset.h"



// TritSet private methods ----------------------

// Return size of set in bytes
size_t TritSet::set_size(const size_t size) {
    return ceil((double) size * 2 / BIT_IN_UINT);
}

// Return numeric value of trit in byte
uint TritSet::trit_value(const Trit trit, const size_t index) {
    size_t num_bits = (index % TRIT_IN_UINT) * 2;

    switch (trit) {
        case False:
            return pow(2, BIT_IN_UINT - num_bits - 2);
        case True:
            return pow(2, BIT_IN_UINT - num_bits - 1);
        case Unknown:
            return 0;
    }
    return 0;
}

// Change number of trit
void TritSet::count_trit(const Trit trit, const CountType type_) {
    switch (trit) {
        case True:
            if (type_ == add)
                num_true++;
            else
                num_true--;
            return;
        case False:
            if (type_ == add)
                num_false++;
            else
                num_false--;
            return;
        case Unknown:
            return;
    }
}

// Extend size of set in TritSet
void TritSet::extend_size(const size_t new_size) {
    size_t new_mem_size = set_size(new_size);

    if (!new_mem_size) {
        set = nullptr;
        mem_size = 0;
        num_size = 0;
        return;
    }

    uint *new_set = new uint[new_mem_size]();

    if (set)
        memmove(new_set, set, mem_size * sizeof(uint));

    delete[] set;
    set = new_set;
    mem_size = new_mem_size;
    num_size = new_mem_size * TRIT_IN_UINT;
}

// Copy all params from one TritSet to another
void TritSet::copy_class(const TritSet &tritSet) {
    mem_size = tritSet.mem_size;
    num_size = tritSet.num_size;

    num_false = tritSet.num_false;
    num_true = tritSet.num_true;

    basic_size = tritSet.basic_size;
    logical_length = tritSet.logical_length;
}


// Operators ----------------------

TritSet::Proxy TritSet::operator[](const size_t index) {
    return TritSet::Proxy(this, index);
}

TritSet TritSet::operator|(TritSet &set1) const{
    Trit t1, t2;
    size_t size1 = this->capacity(), size2 = set1.capacity();

    TritSet result(std::max(size1, size2));
    for (size_t i = 0; i < capacity(); i++) {
        t1 = Unknown;
        t2 = Unknown;

        if (size1 >= i)
            t1 = (*this)[i];
        if (size2 >= i)
            t2 = set1[i];

        result[i] = t1 | t2;
    }

    return result;
}

TritSet TritSet::operator&(TritSet &set1) const{
    Trit t1, t2;
    size_t size1 = this->capacity(), size2 = set1.capacity();

    TritSet result(std::max(size1, size2));
    for (size_t i = 0; i < capacity(); i++) {
        t1 = Unknown;
        t2 = Unknown;

        if (size1 >= i)
            t1 = (*this)[i];
        if (size2 >= i)
            t2 = set1[i];

        result[i] = t1 & t2;
    }

    return result;
}

TritSet TritSet::operator~() const{
    TritSet result(capacity());
    for (size_t i = 0; i < capacity(); i++)
        result[i] = ~(*this)[i];

    return result;
}

// Attributes ----------------------

size_t TritSet::capacity() const {
    return num_size;
}

size_t TritSet::cardinality(const Trit value) const {
    switch (value) {
        case True:
            return num_true;
        case False:
            return num_false;
        case Unknown:
            return num_size - num_true - num_false;
    }
    return 0;
}

size_t TritSet::length() const {
    return logical_length;
}

std::unordered_map<Trit, int, std::hash<int> > TritSet::cardinality() const {
    std::unordered_map<Trit, int, std::hash<int>> result = {
            {Unknown, num_size - num_true - num_false},
            {False,   num_false},
            {True,    num_true}};
    return result;
}

// Public methods ----------------------

void TritSet::shrink() {
    size_t last_byte;
    for (last_byte = mem_size - 1; last_byte > 0; last_byte--) {
        if (set[last_byte])
            break;
    }
    if (set[last_byte])
        last_byte++;

    size_t new_size = std::max(last_byte * TRIT_IN_UINT, basic_size);
    size_t new_mem_size = set_size(new_size);
    if (!new_mem_size) {
        set = nullptr;
        mem_size = 0;
        num_size = 0;
        return;
    }
    uint *new_set = new uint[new_mem_size]();

    if (set)
        memmove(new_set, set, new_mem_size * sizeof(uint));

    delete[] set;
    set = new_set;
    mem_size = new_mem_size;
    num_size = new_size;
}

void TritSet::trim(size_t lastIndex) {
    for (; lastIndex < num_size; lastIndex++)
        (*this)[lastIndex] = Unknown;
}

TritSet::TritSet(size_t size) : basic_size(size) {
    mem_size = set_size(size);
    num_size = mem_size * TRIT_IN_UINT;
    set = nullptr;

    num_false = 0;
    num_true = 0;
    logical_length = 0;
}

TritSet::TritSet(const TritSet &tritSet) {
    copy_class(tritSet);

    if (mem_size) {
        set = new uint[mem_size];
        if (tritSet.set)
            memmove(set, tritSet.set, mem_size * sizeof(uint));
    }
    else
        set = nullptr;
}

TritSet::~TritSet() {
    delete[] set;
}

// Move constructor
TritSet::TritSet(TritSet &&set1) {
    copy_class(set1);

    if (mem_size) {
        set = new uint[mem_size];
        if (set1.set)
            memmove(set, set1.set, mem_size * sizeof(uint));
    }
    else
        set = nullptr;
}

// Proxy operators ----------------------

TritSet::Proxy::Proxy(TritSet *trit_set, size_t ind) : tritSet(trit_set), index(ind) {}

void TritSet::Proxy::operator=(const Trit trit) {
    Trit now = *this;

    if ((trit == Unknown) && (this->tritSet->num_size < index))
        return;
    if (now == trit)
        return;
    tritSet->count_trit(now, del);
    tritSet->count_trit(trit, add);

    // If first allocate
    if (!tritSet->set && tritSet->mem_size)
        tritSet->set = new uint[tritSet->mem_size]();

    // Need new allocate
    if (tritSet->num_size + 1 < index) {
        tritSet->extend_size(index + 1);
    }

    size_t num_byte = index / TRIT_IN_UINT;

    uint now_value = trit_value(now, index);
    uint new_value = trit_value(trit, index);
    tritSet->set[num_byte] = tritSet->set[num_byte] + new_value - now_value;


    if (trit != Unknown)
        tritSet->logical_length = std::max(tritSet->logical_length, index + 1);

        // find new logical_length
    else if (index + 1 == tritSet->logical_length) {
        for (; tritSet->logical_length > 0; tritSet->logical_length--) {
            if ((*tritSet)[tritSet->logical_length]) {
                tritSet->logical_length++;
                break;
            }
        }
    }

}

TritSet::Proxy::operator const Trit() {
    if ((!this->tritSet->set) || (this->tritSet->num_size < index))
        return Unknown;

    size_t num_byte = index / TRIT_IN_UINT;
    size_t num_bits = (index % TRIT_IN_UINT) * 2;


    size_t ans = (this->tritSet->set[num_byte] << num_bits);
    ans = ans >> (BIT_IN_UINT - 2);
    return Trit(ans);
}

// Iterator ----------------------
TritSet::Iterator::Iterator(TritSet *trit_set, size_t index) : tritSet(trit_set), index_now(index) {}

TritSet::Iterator TritSet::Iterator::operator++() {
    return TritSet::Iterator(tritSet, index_now++);
}

TritSet::Iterator TritSet::Iterator::operator--() {
    return TritSet::Iterator(tritSet, index_now--);
}

bool TritSet::Iterator::operator==(const TritSet::Iterator &iterator) const {
    if (tritSet != iterator.tritSet)
        return false;
    if (index_now != iterator.index_now)
        return false;
    return true;
}

bool TritSet::Iterator::operator!=(const TritSet::Iterator &iterator) const {
    return !(*this == iterator);
}

TritSet::Proxy TritSet::Iterator::operator*() {
    return (*tritSet)[index_now];
}

TritSet::Iterator TritSet::begin() {
    return TritSet::Iterator(this, 0);
}

TritSet::Iterator TritSet::end() {
    return TritSet::Iterator(this, num_size);
}

Trit TritSet::operator[](const size_t index) const {
    if (!set || num_size < index)
        return Unknown;

    size_t num_byte = index / TRIT_IN_UINT;
    size_t num_bits = (index % TRIT_IN_UINT) * 2;


    size_t ans = (set[num_byte] << num_bits);
    ans = ans >> (BIT_IN_UINT - 2);
    return Trit(ans);
}

// Test func

TritSet id(TritSet set){
    return set;
}