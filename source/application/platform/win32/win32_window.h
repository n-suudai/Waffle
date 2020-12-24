#pragma once


#include "application/window.h"
#include "common/platform/win32.h"


namespace waffle {
namespace application {


class Win32Window final : public IWindow
{
public:
    static bool createUnique(
        const Rectangle<wfl::int32_t>& clientRect,
        UniquePtr<IWindow>& outWindow);

    static bool createShared(
        const Rectangle<wfl::int32_t>& clientRect,
        SharedPtr<IWindow>& outWindow);

public:
    Win32Window();

    ~Win32Window();

    bool isAlive() const override;

    bool messagePump() override;

    bool setTitle(const StringView& title) override;

    bool setClientRect(const Rectangle<wfl::int32_t>& clientRect) override;

    void* windowHandle() const override;

    void* applicationHandle() const override;

private:
    static LRESULT CALLBACK windowProcedureEntry(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT CALLBACK windowProcedureBody(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    bool initialize(const Rectangle<wfl::int32_t>& clientRect);

    bool registerWindowClass();

    bool unregisterWindowClass();

    bool adjustWindowRect(DWORD style);

    bool createWindow(DWORD style);

    bool destroyWindow();

    bool updateRectangles();

    bool updateClientRect();

    bool updateWindowRect();

private:
    Rectangle<wfl::int32_t> m_clientRect;
    Rectangle<wfl::int32_t> m_windowRect;
    HINSTANCE m_hInstance;
    HWND m_hWindow;
    DWORD m_style;
};


} // namespace application
} // namespace waffle

