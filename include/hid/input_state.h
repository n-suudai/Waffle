#pragma once

#include "common/utility/no_new_delete.h"
#include "memory/stl.h"
#include <array>


namespace waffle {
namespace hid {


enum class InputStateType : bool
{
	Released = false
	, Pressed = true
};


class InputState final
{
	WFL_NO_NEW_DELETE;
public:
	inline constexpr InputState()
		: m_type(InputStateType::Released)
	{}

	inline constexpr InputState(InputStateType type)
		: m_type(type)
	{}

	inline constexpr bool is(InputStateType type) const
	{
		return m_type == type;
	}

	inline constexpr void set(InputStateType type)
	{
		m_type = type;
	}

	inline constexpr StringView toStringView() const
	{
		constexpr const char* Released = "Released";
		constexpr const char* Pressed = "Pressed";

		return m_type == InputStateType::Pressed ? Pressed : Released;
	}

	inline String toString() const
	{
		return String(toStringView());
	}

	inline constexpr explicit operator InputStateType() const
	{
		return m_type;
	}

private:
	InputStateType m_type;
};


} // namespace hid
} // namespace waffle

