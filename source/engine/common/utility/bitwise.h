#pragma once

#include "../types.h"


class Bitwise
{
public:

    // 2�ׂ̂���ł��邩
    template<typename T>
    static inline EBool IsPowerOfTwo(T x)
    {
        return (x & (x - 1)) == 0;
    }
};

