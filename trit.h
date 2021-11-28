#ifndef TRITS_TRIT_H
#define TRITS_TRIT_H

enum Trit {Unknown, False, True};

Trit operator~(const Trit& trit);
Trit operator&(const Trit& trit1, const Trit& trit2);
Trit operator|(const Trit& trit1, const Trit& trit2);

#endif //TRITS_TRIT_H