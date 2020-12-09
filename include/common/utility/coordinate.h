#pragma once


#include "common/types.h"
#include "common/utility/no_new_delete.h"


namespace waffle {


template<typename T>
class Coordinate2D final
{
    WFL_NO_NEW_DELETE;
public:
    inline constexpr Coordinate2D()
        : Coordinate2D(T(0), T(0))
    {}

    inline constexpr Coordinate2D(T x, T y)
        : m_X(x), m_Y(y)
    {}

    inline constexpr Coordinate2D(const Coordinate2D&) = default;

    inline constexpr T x() const { return m_X; }

    inline constexpr T y() const { return m_Y; }

private:
    T m_X;
    T m_Y;
};



} // namespace waffle

