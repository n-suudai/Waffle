#pragma once

#include "common/utility/no_new_delete.h"
#include "memory/stl.h"
#include <array>


namespace waffle {
namespace hid {


enum class MouseAxisType : wfl::uint8_t
{
	Axis_X
	, Axis_Y
	, Axis_Scroll
	, Axis_Num
};


class MouseAxis final
{
	WFL_NO_NEW_DELETE;
public:
	static constexpr wfl::size_t MAX_NUM = static_cast<wfl::size_t>(MouseAxisType::Axis_Num);

	inline constexpr MouseAxis()
		: m_type(MouseAxisType::Axis_X)
	{}

	inline constexpr MouseAxis(MouseAxisType type)
		: m_type(type)
	{}

	inline constexpr bool is(MouseAxisType type) const
	{
		return m_type == type;
	}

	inline constexpr bool isValid() const
	{
		return MouseAxisType::Axis_X <= m_type && m_type < MouseAxisType::Axis_Num;
	}

	inline constexpr wfl::size_t toIndex() const
	{
		return static_cast<wfl::size_t>(m_type);
	}

	inline constexpr StringView toStringView() const
	{
		constexpr wfl::size_t MOUSE_AXIS_NUM = static_cast<wfl::size_t>(MouseAxisType::Axis_Num);

		typedef wfl::array<const char*, MOUSE_AXIS_NUM> ToStringTable;

		constexpr ToStringTable TO_STRING_TABLE = {
				"Axis_X"
				, "Axis_Y"
				, "Axis_Scroll"
		};

		return TO_STRING_TABLE[static_cast<wfl::size_t>(m_type)];
	}

	inline String toString() const
	{
		return String(toStringView());
	}

	inline constexpr explicit operator MouseAxisType() const
	{
		return m_type;
	}

private:
	MouseAxisType m_type;
};


} // namespace hid
} // namespace waffle

