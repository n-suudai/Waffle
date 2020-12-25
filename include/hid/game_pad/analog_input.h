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
	inline constexpr AnalogInput()
		: m_type(AnalogInputType::LeftThumbStickX)
	{}

	inline constexpr AnalogInput(AnalogInputType type)
		: m_type(type)
	{}

	inline constexpr bool is(AnalogInputType type) const
	{
		return m_type == type;
	}

	inline constexpr StringView toStringView() const
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

	inline String toString() const
	{
		return String(toStringView());
	}

	inline constexpr explicit operator AnalogInputType() const
	{
		return m_type;
	}

private:
	AnalogInputType m_type;
};


} // namespace hid
} // namespace waffle

