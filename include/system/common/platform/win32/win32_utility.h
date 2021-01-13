#pragma once


#include "system/common/utility/rectangle.h"
#include "system/common/platform/win32/win32_prerequisites.h"


namespace waffle {


[[nodiscard]]
inline RECT convertRect(const Rectangle<wfl::int32_t>& rectangle)
{
    RECT result = {};
    rectangle.getHorizontal(result.left, result.right);
    rectangle.getVertical(result.top, result.bottom);
    return result;
}

[[nodiscard]]
inline Rectangle<wfl::int32_t> convertRect(const RECT& rect)
{
    Rectangle<wfl::int32_t> result(
        rect.left, rect.right,
        rect.top, rect.bottom
    );
    return result;
}

[[nodiscard]]
inline bool getClientRect(HWND hWindow, Rectangle<wfl::int32_t>& outClientRect)
{
    RECT clientRect = {};
    if (!::GetClientRect(hWindow, &clientRect))
    {
        return false;
    }

    outClientRect = convertRect(clientRect);
    return true;
}

[[nodiscard]]
inline bool getWindowRect(HWND hWindow, Rectangle<wfl::int32_t>& outClientRect)
{
    RECT clientRect = {};
    if (!::GetWindowRect(hWindow, &clientRect))
    {
        return false;
    }

    outClientRect = convertRect(clientRect);
    return true;
}


} // namespace waffle

