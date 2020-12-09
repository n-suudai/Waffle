
#include "win32_window.h"
#include "core/logging.h"


namespace waffle {
namespace application {


memory::UniquePtr<IWindow> Win32Window::createUnique(const Rectangle<wfl::uint32_t>& clientRect)
{
    return WFL_MAKE_UNIQUE(Win32Window, clientRect);
}

memory::SharedPtr<IWindow> Win32Window::createShared(const Rectangle<wfl::uint32_t>& clientRect)
{
    return WFL_MAKE_SHARED(Win32Window, clientRect);
}


Win32Window::Win32Window(const Rectangle<wfl::uint32_t>& clientRect)
    : m_clientRect(clientRect)
{
    logging::put("Win32Window()");
}

Win32Window::~Win32Window()
{
    logging::put("~Win32Window()");
}

bool Win32Window::IsAlive() const
{
    return true;
}


} // namespace application
} // namespace waffle

