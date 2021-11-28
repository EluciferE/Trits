#include "trit.h"



Trit operator~(const Trit& trit){
    if (trit == True)
        return Trit(False);
    if (trit == Unknown)
        return Trit(Unknown);
    return Trit(True);
}

Trit operator&(const Trit& trit1, const Trit& trit2){
    if ((trit1 == False) || (trit2 == False))
        return Trit(False);
    if ((trit1 == True) && (trit2 == True))
        return Trit(True);
    return Trit(Unknown);
}

Trit operator|(const Trit& trit1, const Trit& trit2){
    if ((trit1 == True) || (trit2 == True))
        return Trit(True);
    if ((trit1 == False) && (trit2 == False))
        return Trit(False);
    return Trit(Unknown);
}

