#pragma once

#include "../types.h"


class Bitwise
{
public:

    // 2のべき乗であるか
    template<typename T>
    static inline EBool IsPowerOfTwo(T x)
    {
        return (x & (x - 1)) == 0;
    }
};

