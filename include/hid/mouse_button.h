#pragma once

#include "common/utility/no_new_delete.h"
#include "memory/stl.h"
#include <array>


namespace waffle {
namespace hid {


enum class MouseButtonType : wfl::uint8_t
{
	Button_0
	, Button_1
	, Button_2
	, Button_3
	, Button_4
	, Button_5
	, Button_6
	, Button_7
	, Button_Num
};


class MouseButton final
{
	WFL_NO_NEW_DELETE;
public:
	inline constexpr MouseButton()
		: m_type(MouseButtonType::Button_0)
	{}

	inline constexpr explicit MouseButton(MouseButtonType type)
		: m_type(type)
	{}

	inline constexpr bool is(MouseButtonType type) const
	{
		return m_type == type;
	}

	inline constexpr StringView toStringView() const
	{
		constexpr wfl::size_t MOUSE_BUTTON_NUM = static_cast<wfl::size_t>(MouseButtonType::Button_Num);

		typedef wfl::array<const char*, MOUSE_BUTTON_NUM> ToStringTable;

		constexpr ToStringTable TO_STRING_TABLE = {
				"Mouse_Button_0"
				, "Mouse_Button_1"
				, "Mouse_Button_2"
				, "Mouse_Button_3"
				, "Mouse_Button_4"
				, "Mouse_Button_5"
				, "Mouse_Button_6"
				, "Mouse_Button_7"
		};

		return TO_STRING_TABLE[static_cast<wfl::size_t>(m_type)];
	}

	inline String toString() const
	{
		return String(toStringView());
	}

	inline constexpr explicit operator MouseButtonType() const
	{
		return m_type;
	}

private:
	MouseButtonType m_type;
};


} // namespace hid
} // namespace waffle

