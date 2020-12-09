
#include "application/window.h"

#include "platform/win32/win32_window.h"



namespace waffle {
namespace application {


memory::UniquePtr<IWindow> createWindowUnique(const Rectangle<wfl::int32_t>& clientRect)
{
    return Win32Window::createUnique(clientRect);
}

memory::SharedPtr<IWindow> createWindowShared(const Rectangle<wfl::int32_t>& clientRect)
{
    return Win32Window::createShared(clientRect);
}


} // namespace application
} // namespace waffle

