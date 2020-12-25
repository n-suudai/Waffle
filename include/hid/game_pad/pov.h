#pragma once

#include "common/utility/no_new_delete.h"
#include "memory/stl.h"
#include <array>


namespace waffle {
namespace hid {


enum class POVType : wfl::uint8_t
{
	POV_0
	, POV_1
	, POV_2
	, POV_3
	, POV_4
	, POV_5
	, POV_6
	, POV_7
	, POV_8
	, POV_Num
};


class POV final
{
	WFL_NO_NEW_DELETE;
public:
	inline constexpr POV()
		: m_type(POVType::POV_0)
	{}

	inline constexpr POV(POVType type)
		: m_type(type)
	{}

	inline constexpr bool is(POVType type) const
	{
		return m_type == type;
	}

	inline constexpr StringView toStringView() const
	{
		constexpr wfl::size_t POV_BUTTON_NUM = static_cast<wfl::size_t>(POVType::POV_Num);

		typedef wfl::array<const char*, POV_BUTTON_NUM> ToStringTable;

		constexpr ToStringTable TO_STRING_TABLE = {
				"POV_0"
				, "POV_1"
				, "POV_2"
				, "POV_3"
				, "POV_4"
				, "POV_5"
				, "POV_6"
				, "POV_7"
				, "POV_8"
		};

		return TO_STRING_TABLE[static_cast<wfl::size_t>(m_type)];
	}

	inline String toString() const
	{
		return String(toStringView());
	}

	inline constexpr explicit operator POVType() const
	{
		return m_type;
	}

private:
	POVType m_type;
};


} // namespace hid
} // namespace waffle

