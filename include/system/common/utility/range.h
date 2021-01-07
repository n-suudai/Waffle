#pragma once


#include "system/common/types.h"
#include "system/common/utility/no_new_delete.h"
#include <cassert>
#include <cmath>


namespace waffle {


template <typename T>
[[nodiscard]] inline T clamp(T x, T low, T high)
{
    assert(low <= high);
    return wfl::min(wfl::max(x, low), high);
}

template <typename T>
[[nodiscard]] inline T wrap(T x, T low, T high)
{
    assert(low < high);
    const T n = (x - low) % (high - low);
    return (n >= 0) ? (n + low) : (n + high);
}

// float 特殊化
template <>
[[nodiscard]] inline float wrap<float>(float x, float low, float high)
{
    assert(low < high);
    const float n = wfl::fmod(x - low, high - low);
    return (n >= 0) ? (n + low) : (n + high);
}

// double 特殊化
template <>
[[nodiscard]] inline double wrap<double>(double x, double low, double high)
{
    assert(low < high);
    const double n = wfl::fmod(x - low, high - low);
    return (n >= 0) ? (n + low) : (n + high);
}

// Range
template <typename T>
class Range final
{
    WFL_NO_NEW_DELETE;
public:
    inline constexpr Range()
        : Range(T(0), T(1))
    {}

    inline constexpr Range(T min, T max)
        : m_min(min), m_max(max)
    {
        assert(m_min <= m_max);
    }

    inline constexpr Range(const Range&) = default;

    [[nodiscard]] inline constexpr bool isInside(T x) const
    {
        return (m_min <= x) && (x <= m_max);
    }

    [[nodiscard]] inline constexpr T wrap(T x) const
    {
        return waffle::wrap(x, m_min, m_max);
    }

    [[nodiscard]] inline constexpr T clamp(T x) const
    {
        return waffle::clamp(x, m_min, m_max);
    }

    [[nodiscard]] inline constexpr T min() const
    {
        return m_min;
    }

    [[nodiscard]] inline constexpr T max() const
    {
        return m_max;
    }

private:
    T m_min;
    T m_max;
};


} // namespace waffle

