#include "modules/application/window.h"
#include "platform/win32/win32_window.h"


namespace waffle {
namespace application {


bool createWindowUnique(
    const Rectangle<wfl::int32_t>& clientRect,
    UniquePtr<IWindow>& outWindow)
{
    return Win32Window::createUnique(
        clientRect,
        outWindow);
}

bool createWindowShared(
    const Rectangle<wfl::int32_t>& clientRect,
    SharedPtr<IWindow>& outWindow)
{
    return Win32Window::createShared(
        clientRect,
        outWindow);
}


} // namespace application
} // namespace waffle

