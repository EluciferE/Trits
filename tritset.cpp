#include "tritset.h"


TritSet::TritSet(size_t size): num_size(size){
    this->mem_size = set_size(size);
    this->set = nullptr;

    this->num_false = 0;
    this->num_true = 0;
}

size_t TritSet::set_size(size_t size) {
    return ceil((double)size * 2 / (8 * sizeof(uint)));;
}


uint TritSet::trit_value(Trit trit, size_t index) {
    size_t num_bits = (index % (sizeof(uint) * 4)) * 2;

    switch (trit){
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
    return this->num_size;
}


TritSet::Proxy::Proxy(TritSet *trit_set, size_t ind): tritSet(trit_set), index(ind){}

void TritSet::Proxy::operator=(const Trit trit) {
    Trit now = *this;

    if ((trit == Unknown) && (this->tritSet->num_size < index))
        return;
    if (now == trit)
        return;

    // If first allocate
    if (!tritSet->set){
        this->tritSet->set = new uint[this->tritSet->mem_size];
        memset(this->tritSet->set, 0, this->tritSet->mem_size * sizeof(uint));
    }

    // Need new allocate
    if (this->tritSet->num_size < (index - 1)){
        extend_set(index + 1);
    }

    size_t num_byte = index/(sizeof(uint) * 4);

    uint now_value = trit_value(now, index);
    uint new_value = trit_value(trit, index);
    this->tritSet->set[num_byte] = this->tritSet->set[num_byte] + new_value - now_value;

}

TritSet::Proxy::operator const Trit() {
    if ((!this->tritSet->set) || (this->tritSet->num_size < index))
        return Unknown;

    size_t num_byte = index/(sizeof(uint) * 4);
    size_t num_bits = (index % (sizeof(uint) * 4)) * 2;


    size_t ans = (this->tritSet->set[num_byte] << num_bits);
    ans = ans >> (sizeof(uint) * 8 - 2);
    return Trit(ans);
}

void TritSet::Proxy::extend_set(int new_size) {

    size_t new_mem_size = set_size(new_size);
    uint* new_set = new uint[new_mem_size];

    memset(new_set, 0, new_mem_size * sizeof(uint));
    memmove(new_set, this->tritSet->set, this->tritSet->mem_size * sizeof(uint));
    this->tritSet->set = new_set;
    this->tritSet->mem_size = new_mem_size;
    this->tritSet->num_size = new_size;
}
