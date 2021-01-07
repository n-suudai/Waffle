#pragma once

#include "system/common/utility/no_new_delete.h"
#include "system/memory/stl/stl.h"
#include <array>


namespace waffle {
namespace hid {


enum class GamePadButtonType : wfl::uint8_t
{
	Button_0
	, Button_1
	, Button_2
	, Button_3
	, Button_4
	, Button_5
	, Button_6
	, Button_7
	, Button_8
	, Button_9
	, Button_10
	, Button_11
	, Button_12
	, Button_13
	, Button_14
	, Button_Num
};


class GamePadButton final
{
	WFL_NO_NEW_DELETE;
public:
	static constexpr wfl::size_t MAX_NUM = static_cast<wfl::size_t>(GamePadButtonType::Button_Num);

	inline constexpr GamePadButton()
		: m_type(GamePadButtonType::Button_0)
	{}

	inline constexpr GamePadButton(GamePadButtonType type)
		: m_type(type)
	{}

	[[nodiscard]] inline constexpr bool is(GamePadButtonType type) const
	{
		return m_type == type;
	}

	[[nodiscard]] inline constexpr bool isValid() const
	{
		return GamePadButtonType::Button_0 <= m_type && m_type < GamePadButtonType::Button_Num;
	}

	[[nodiscard]] inline constexpr wfl::size_t toIndex() const
	{
		return static_cast<wfl::size_t>(m_type);
	}

	[[nodiscard]] inline constexpr StringView toStringView() const
	{
		constexpr wfl::size_t GAME_PAD_BUTTON_NUM = static_cast<wfl::size_t>(GamePadButtonType::Button_Num);

		typedef wfl::array<const char*, GAME_PAD_BUTTON_NUM> ToStringTable;

		constexpr ToStringTable TO_STRING_TABLE = {
				"GamePad_Button_0"
				, "GamePad_Button_1"
				, "GamePad_Button_2"
				, "GamePad_Button_3"
				, "GamePad_Button_4"
				, "GamePad_Button_5"
				, "GamePad_Button_6"
				, "GamePad_Button_7"
				, "GamePad_Button_8"
				, "GamePad_Button_9"
				, "GamePad_Button_10"
				, "GamePad_Button_11"
				, "GamePad_Button_12"
				, "GamePad_Button_13"
				, "GamePad_Button_14"
		};

		return TO_STRING_TABLE[static_cast<wfl::size_t>(m_type)];
	}

	[[nodiscard]] inline String toString() const
	{
		return String(toStringView());
	}

	[[nodiscard]] inline constexpr explicit operator GamePadButtonType() const
	{
		return m_type;
	}

private:
	GamePadButtonType m_type;
};


} // namespace hid
} // namespace waffle

