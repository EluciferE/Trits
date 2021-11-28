#include "tritset.h"


TritSet::TritSet(size_t size) : num_size(size) {
    mem_size = set_size(size);
    set = nullptr;

    num_false = 0;
    num_true = 0;
    last_trit = -1;
}

size_t TritSet::set_size(size_t size) {
    return ceil((double) size * 2 / (8 * sizeof(uint)));;
}


uint TritSet::trit_value(Trit trit, size_t index) {
    size_t num_bits = (index % (sizeof(uint) * 4)) * 2;

    switch (trit) {
        case False:
            return pow(2, sizeof(uint) * 8 - num_bits - 2);
        case True:
            return pow(2, sizeof(uint) * 8 - num_bits - 1);
        case Unknown:
            return 0;
    }
    return 0;
}


TritSet::Proxy TritSet::operator[](size_t index) {
    return TritSet::Proxy(this, index);
}

size_t TritSet::capacity() {
    return num_size;
}

size_t TritSet::cardinality(Trit value) {
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

void TritSet::count_trit(Trit trit, CountType type_) {
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

void TritSet::equal_sizes(TritSet &set1) {
    if (capacity() > set1.capacity())
        set1.extend_size(capacity());
    else if (capacity() < set1.capacity())
        extend_size(set1.capacity());
    return;
}

void TritSet::extend_size(size_t new_size) {
    size_t new_mem_size = set_size(new_size);
    uint *new_set = new uint[new_mem_size];

    memset(new_set, 0, new_mem_size * sizeof(uint));

    if (set)
        memmove(new_set, set, mem_size * sizeof(uint));

    set = new_set;
    mem_size = new_mem_size;
    num_size = new_size;

}


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
    if (!tritSet->set) {
        tritSet->set = new uint[tritSet->mem_size];
        memset(tritSet->set, 0, tritSet->mem_size * sizeof(uint));
    }

    // Need new allocate
    if (tritSet->num_size < (index - 1)) {
        tritSet->extend_size(index + 1);
    }

    size_t num_byte = index / (sizeof(uint) * 4);

    uint now_value = trit_value(now, index);
    uint new_value = trit_value(trit, index);
    tritSet->set[num_byte] = tritSet->set[num_byte] + new_value - now_value;

    if (trit != Unknown)
        tritSet->last_trit = max(tritSet->last_trit, index);

}

TritSet::Proxy::operator const Trit() {
    if ((!this->tritSet->set) || (this->tritSet->num_size < index))
        return Unknown;

    size_t num_byte = index / (sizeof(uint) * 4);
    size_t num_bits = (index % (sizeof(uint) * 4)) * 2;


    size_t ans = (this->tritSet->set[num_byte] << num_bits);
    ans = ans >> (sizeof(uint) * 8 - 2);
    return Trit(ans);
}
// Iterator
TritSet::Iterator::Iterator(TritSet *trit_set, size_t index): tritSet(trit_set), index_now(index){}

TritSet::Iterator TritSet::Iterator::operator++() {
    index_now++;
    return TritSet::Iterator(tritSet, index_now - 1);
}

TritSet::Iterator TritSet::Iterator::operator--() {
    index_now--;
    return TritSet::Iterator(tritSet, index_now + 1);
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


TritSet TritSet::operator|(TritSet &set1) {
    equal_sizes(set1);

    Trit t1, t2;
    TritSet result(capacity());
    for (size_t i = 0; i < capacity(); i++){
        t1 = (*this)[i];
        t2 = set1[i];
        result[i] = t1 | t2;
    }

    return result;
}

TritSet TritSet::operator&(TritSet &set1) {
    equal_sizes(set1);

    Trit t1, t2;
    TritSet result(capacity());
    for (size_t i = 0; i < capacity(); i++){
        t1 = (*this)[i];
        t2 = set1[i];
        result[i] = t1 & t2;
    }

    return result;
}

TritSet TritSet::operator~() {
    TritSet result(capacity());
    for (size_t i = 0; i < capacity(); i++)
        result[i] = ~(*this)[i];

    return result;
}
