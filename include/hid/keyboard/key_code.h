#pragma once

#include "common/utility/no_new_delete.h"
#include "memory/stl.h"
#include <array>


namespace waffle {
namespace hid {


enum class KeyCodeType : wfl::uint8_t
{
    Key_Unknown

    , Key_A
    , Key_B
    , Key_C
    , Key_D
    , Key_E
    , Key_F
    , Key_G
    , Key_H
    , Key_I
    , Key_J
    , Key_K
    , Key_L
    , Key_M
    , Key_N
    , Key_O
    , Key_P
    , Key_Q
    , Key_R
    , Key_S
    , Key_T
    , Key_U
    , Key_V
    , Key_W
    , Key_X
    , Key_Y
    , Key_Z

    , Key_1
    , Key_2
    , Key_3
    , Key_4
    , Key_5
    , Key_6
    , Key_7
    , Key_8
    , Key_9
    , Key_0

    , Key_F1
    , Key_F2
    , Key_F3
    , Key_F4
    , Key_F5
    , Key_F6
    , Key_F7
    , Key_F8
    , Key_F9
    , Key_F10
    , Key_F11
    , Key_F12

    , Key_Escape
    , Key_Return
    , Key_Space
    , Key_BackSpace
    , Key_Delete
    , Key_Tab

    , Key_LeftControl
    , Key_LeftShift
    , Key_LeftAlt

    , Key_RightControl
    , Key_RightShift
    , Key_RightAlt

    , Key_Up
    , Key_Down
    , Key_Left
    , Key_Right

    , Key_Numpad1
    , Key_Numpad2
    , Key_Numpad3
    , Key_Numpad4
    , Key_Numpad5
    , Key_Numpad6
    , Key_Numpad7
    , Key_Numpad8
    , Key_Numpad9
    , Key_Numpad0
    , Key_NumpadEnter

    , Key_Minus
    , Key_Equals
    , Key_Multiply
    , Key_Subtract
    , Key_Add
    , Key_Divide
    , Key_Decimal
    , Key_Semicolon
    , Key_Apostrophe
    , Key_Comma
    , Key_Period
    , Key_Grave
    , Key_Slash
    , Key_BackSlash

    , Key_LeftBracket
    , Key_RightBracket

    , Key_PageUp
    , Key_PageDown
    , Key_End
    , Key_SysRq
    , Key_Pause
    , Key_Home
    , Key_Capital
    , Key_NumLock
    , Key_Scroll
    , Key_Insert
    , Key_Apps

    , Key_LeftWin
    , Key_RightWin

    , Key_Num
};


class KeyCode final
{
    WFL_NO_NEW_DELETE;
public:
    static constexpr wfl::size_t MAX_NUM = static_cast<wfl::size_t>(KeyCodeType::Key_Num);

    inline constexpr KeyCode()
        : m_type(KeyCodeType::Key_Unknown)
    {}

    inline constexpr KeyCode(KeyCodeType type)
        : m_type(type)
    {}

    inline constexpr bool is(KeyCodeType type) const
    {
        return m_type == type;
    }

    inline constexpr bool isValid() const
    {
        return KeyCodeType::Key_Unknown <= m_type && m_type < KeyCodeType::Key_Num;
    }

    inline constexpr wfl::size_t toIndex() const
    {
        return static_cast<wfl::size_t>(m_type);
    }

    inline constexpr StringView toStringView() const
    {
        constexpr wfl::size_t KEY_CODE_NUM = static_cast<wfl::size_t>(KeyCodeType::Key_Num);

        typedef wfl::array<const char*, KEY_CODE_NUM> ToStringTable;

        constexpr ToStringTable TO_STRING_TABLE = {
            "Unknown"

            , "A"
            , "B"
            , "C"
            , "D"
            , "E"
            , "F"
            , "G"
            , "H"
            , "I"
            , "J"
            , "K"
            , "L"
            , "M"
            , "N"
            , "O"
            , "P"
            , "Q"
            , "R"
            , "S"
            , "T"
            , "U"
            , "V"
            , "W"
            , "X"
            , "Y"
            , "Z"

            , "1"
            , "2"
            , "3"
            , "4"
            , "5"
            , "6"
            , "7"
            , "8"
            , "9"
            , "0"

            , "F1"
            , "F2"
            , "F3"
            , "F4"
            , "F5"
            , "F6"
            , "F7"
            , "F8"
            , "F9"
            , "F10"
            , "F11"
            , "F12"

            , "Escape"
            , "Return"
            , "Space"
            , "BackSpace"
            , "Delete"
            , "Tab"

            , "LeftControl"
            , "LeftShift"
            , "LeftAlt"

            , "RightControl"
            , "RightShift"
            , "RightAlt"

            , "Up"
            , "Down"
            , "Left"
            , "Right"

            , "NumPad1"
            , "NumPad2"
            , "NumPad3"
            , "NumPad4"
            , "NumPad5"
            , "NumPad6"
            , "NumPad7"
            , "NumPad8"
            , "NumPad9"
            , "NumPad0"
            , "NumPadEnter"

            , "Minus"
            , "Equals"
            , "Multiply"
            , "Subtract"
            , "Add"
            , "Divide"
            , "Decimal"
            , "Semicolon"
            , "Apostrophe"
            , "Comma"
            , "Period"
            , "Grave"
            , "Slash"
            , "BackSlash"

            , "LeftBracket"
            , "RightBracket"

            , "PageUp"
            , "PageDown"
            , "End"
            , "SysRq"
            , "Pause"
            , "Home"
            , "Capital"
            , "NumLock"
            , "Scroll"
            , "Insert"
            , "Apps"

            , "LeftWin"
            , "RightWin"
        };

        return StringView(TO_STRING_TABLE[static_cast<wfl::size_t>(m_type)]);
    }

    inline String toString() const
    {
        return String(toStringView());
    }

    inline constexpr explicit operator KeyCodeType() const
    {
        return m_type;
    }

private:
    KeyCodeType m_type;
};


} // namespace hid
} // namespace waffle

