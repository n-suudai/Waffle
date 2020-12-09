#pragma once


#include "common/utility/range.h"
#include "common/utility/coordinate.h"


namespace waffle {


template<typename T>
class Rectangle final
{
    WFL_NO_NEW_DELETE;
public:
    typedef Range<T> RangeType;
    typedef Coordinate2D<T> Coordinate2DType;

    inline constexpr Rectangle()
        : m_horizontalRange()
        , m_verticalRange()
    {}

    inline constexpr Rectangle(const Rectangle&) = default;

    inline constexpr Rectangle(T left, T right, T top, T bottom)
        : m_horizontalRange(left, right)
        , m_verticalRange(top, bottom)
    {}

    inline constexpr Rectangle(
        const RangeType& horizontalRange,
        const RangeType& verticalRange)
        : m_horizontalRange(horizontalRange)
        , m_verticalRange(verticalRange)
    {}

    inline void getHorizontal(T& left, T& right) const
    {
        left = m_horizontalRange.min();
        right = m_horizontalRange.max();
    }

    template<typename Cast>
    inline void getHorizontal(Cast& leftCast, Cast& rightCast) const
    {
        T left, right;
        getHorizontal(left, right);
        leftCast = static_cast<Cast>(left);
        rightCast = static_cast<Cast>(right);
    }

    inline void getVertical(T& top, T& bottom) const
    {
        top = m_verticalRange.min();
        bottom = m_verticalRange.max();
    }

    template<typename Cast>
    inline void getVertical(Cast& topCast, Cast& bottomCast) const
    {
        T top, bottom;
        getVertical(top, bottom);
        topCast = static_cast<Cast>(top);
        bottomCast = static_cast<Cast>(bottomCast);
    }

    inline constexpr bool isInside(const Coordinate2DType& coordinate) const
    {
        return m_horizontalRange.isInside(coordinate.x())
            && m_verticalRange.isInside(coordinate.y());
    }

private:
    RangeType m_horizontalRange;
    RangeType m_verticalRange;
};


} // namespace waffle

