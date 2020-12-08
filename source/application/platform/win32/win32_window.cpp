
#include "win32_window.h"
#include "core/logging.h"


namespace waffle {
namespace application {


memory::UniquePtr<IWindow> Win32Window::createUnique()
{
    return WFL_MAKE_UNIQUE(Win32Window);
}

memory::SharedPtr<IWindow> Win32Window::createShared()
{
    return WFL_MAKE_SHARED(Win32Window);
}


Win32Window::Win32Window()
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

