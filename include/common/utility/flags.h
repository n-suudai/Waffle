#pragma once


#include "common/types.h"


namespace waffle {


template <typename FlagBitsType> struct FlagTraits
{
    enum { allFlags = 0 };
};

template <typename BitType, typename MaskType = wfl::uint64_t>
class Flags
{
public:
    inline constexpr Flags()
        : m_mask(0)
    {}

    inline constexpr Flags(BitType bit)
        : m_mask(static_cast<MaskType>(bit))
    {}

    inline constexpr Flags(Flags<BitType> const& rhs)
        : m_mask(rhs.m_mask)
    {}

    inline constexpr explicit Flags(MaskType flags)
        : m_mask(flags)
    {}

    inline Flags<BitType>& operator=(Flags<BitType> const& rhs)
    {
        m_mask = rhs.m_mask;
        return *this;
    }

    inline Flags<BitType>& operator|=(Flags<BitType> const& rhs)
    {
        m_mask |= rhs.m_mask;
        return *this;
    }

    inline Flags<BitType>& operator&=(Flags<BitType> const& rhs)
    {
        m_mask &= rhs.m_mask;
        return *this;
    }

    inline Flags<BitType>& operator^=(Flags<BitType> const& rhs)
    {
        m_mask ^= rhs.m_mask;
        return *this;
    }

    [[nodiscard]] inline constexpr Flags<BitType> operator|(Flags<BitType> const& rhs) const
    {
        return Flags<BitType>(m_mask | rhs.m_mask);
    }

    [[nodiscard]] inline constexpr Flags<BitType> operator&(Flags<BitType> const& rhs) const
    {
        return Flags<BitType>(m_mask & rhs.m_mask);
    }

    [[nodiscard]] inline constexpr Flags<BitType> operator^(Flags<BitType> const& rhs) const
    {
        return Flags<BitType>(m_mask ^ rhs.m_mask);
    }

    [[nodiscard]] inline constexpr bool operator!() const
    {
        return !m_mask;
    }

    [[nodiscard]] inline constexpr Flags<BitType> operator~() const
    {
        return Flags<BitType>(m_mask ^ FlagTraits<BitType>::allFlags);
    }

    [[nodiscard]] inline constexpr bool operator==(Flags<BitType> const& rhs) const
    {
        return m_mask == rhs.m_mask;
    }

    [[nodiscard]] inline constexpr bool operator!=(Flags<BitType> const& rhs) const
    {
        return m_mask != rhs.m_mask;
    }

    [[nodiscard]] inline explicit constexpr operator bool() const
    {
        return !!m_mask;
    }

    [[nodiscard]] inline explicit constexpr operator MaskType() const
    {
        return m_mask;
    }

private:
    MaskType  m_mask;
};

template <typename BitType>
[[nodiscard]] inline constexpr Flags<BitType> operator|(BitType bit, Flags<BitType> const& flags)
{
    return flags | bit;
}

template <typename BitType>
[[nodiscard]] inline constexpr Flags<BitType> operator&(BitType bit, Flags<BitType> const& flags)
{
    return flags & bit;
}

template <typename BitType>
[[nodiscard]] inline constexpr Flags<BitType> operator^(BitType bit, Flags<BitType> const& flags)
{
    return flags ^ bit;
}

template <typename BitType>
[[nodiscard]] inline constexpr bool operator==(BitType bit, Flags<BitType> const& flags)
{
    return flags == bit;
}

template <typename BitType>
[[nodiscard]] inline constexpr bool operator!=(BitType bit, Flags<BitType> const& flags)
{
    return flags != bit;
}


} // namespace waffle

