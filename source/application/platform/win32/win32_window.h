#pragma once


#include "application/window.h"
#include "common/platform/win32.h"


namespace waffle {
namespace application {


class Win32Window final : public IWindow
{
public:
    static memory::UniquePtr<IWindow> createUnique(const Rectangle<wfl::int32_t>& clientRect);

    static memory::SharedPtr<IWindow> createShared(const Rectangle<wfl::int32_t>& clientRect);

public:
    Win32Window(const Rectangle<wfl::int32_t>& clientRect);

    ~Win32Window();

    bool isAlive() const override;

    bool messagePump();

private:
    static LRESULT CALLBACK windowProcedureEntry(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT CALLBACK windowProcedureBody(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    Rectangle<wfl::int32_t> m_clientRect;
    Rectangle<wfl::int32_t> m_windowRect;
    HINSTANCE m_hInstance;
    HWND m_hWindow;
};


} // namespace application
} // namespace waffle

