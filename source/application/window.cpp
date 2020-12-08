
#include "application/window.h"

#include "platform/win32/win32_window.h"



namespace waffle {
namespace application {


memory::UniquePtr<IWindow> createWindowUnique()
{
    return Win32Window::createUnique();
}

memory::SharedPtr<IWindow> createWindowShared()
{
    return Win32Window::createShared();
}


} // namespace application
} // namespace waffle

