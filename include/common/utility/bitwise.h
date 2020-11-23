#pragma once

#include "common/types.h"


namespace waffle {


class Bitwise
{
public:
    // 2‚Ì‚×‚«æ‚Å‚ ‚é‚©
    template<typename T>
    static inline bool isPowerOfTwo(T x)
    {
        return (x & (x - 1)) == 0;
    }
};


} // namespace waffle

