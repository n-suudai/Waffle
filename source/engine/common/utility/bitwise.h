#pragma once

#include "../types.h"


class Bitwise
{
public:

    // 2‚Ì‚×‚«æ‚Å‚ ‚é‚©
    template<typename T>
    static inline EBool IsPowerOfTwo(T x)
    {
        return (x & (x - 1)) == 0;
    }
};

