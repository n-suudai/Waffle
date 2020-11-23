#pragma once

#include "common/types.h"


namespace waffle {


class Bitwise
{
public:
    // 2�ׂ̂���ł��邩
    template<typename T>
    static inline bool isPowerOfTwo(T x)
    {
        return (x & (x - 1)) == 0;
    }
};


} // namespace waffle

