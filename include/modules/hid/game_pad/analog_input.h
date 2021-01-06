#pragma once

#include "common/utility/no_new_delete.h"
#include "memory/stl.h"
#include <array>


namespace waffle {
namespace hid {


enum class AnalogInputType : wfl::uint8_t
{
	LeftThumbStickX
	, LeftThumbStickY
	, RightThumbStickX
	, RightThumbStickY
	, LeftTrigger
	, RightTrigger
	, Num
};


class AnalogInput final
{
	WFL_NO_NEW_DELETE;
public:
	static constexpr wfl::size_t MAX_NUM = static_cast<wfl::size_t>(AnalogInputType::Num);

	inline constexpr AnalogInput()
		: m_type(AnalogInputType::LeftThumbStickX)
	{}

	inline constexpr AnalogInput(AnalogInputType type)
		: m_type(type)
	{}

	[[nodiscard]] inline constexpr bool is(AnalogInputType type) const
	{
		return m_type == type;
	}

	[[nodiscard]] inline constexpr bool isValid() const
	{
		return AnalogInputType::LeftThumbStickX <= m_type && m_type < AnalogInputType::Num;
	}

	[[nodiscard]] inline constexpr wfl::size_t toIndex() const
	{
		return static_cast<wfl::size_t>(m_type);
	}

	[[nodiscard]] inline constexpr StringView toStringView() const
	{
		constexpr wfl::size_t ANALOG_INPUT_NUM = static_cast<wfl::size_t>(AnalogInputType::Num);

		typedef wfl::array<const char*, ANALOG_INPUT_NUM> ToStringTable;

		constexpr ToStringTable TO_STRING_TABLE = {
			"LeftThumbStickX"
			, "LeftThumbStickY"
			, "RightThumbStickX"
			, "RightThumbStickY"
			, "LeftTrigger"
			, "RightTrigger"
		};

		return TO_STRING_TABLE[static_cast<wfl::size_t>(m_type)];
	}

	[[nodiscard]] inline String toString() const
	{
		return String(toStringView());
	}

	[[nodiscard]] inline constexpr explicit operator AnalogInputType() const
	{
		return m_type;
	}

private:
	AnalogInputType m_type;
};


} // namespace hid
} // namespace waffle

