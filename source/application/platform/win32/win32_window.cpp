
#include "win32_window.h"
#include "core/logging.h"


namespace waffle {
namespace application {

constexpr const char* WINDOW_CLASS_NAME = "WAFFLE_WINDOW";

static RECT convertRect(const Rectangle<wfl::int32_t>& rectangle)
{
    RECT rect = {};
    rectangle.getHorizontal(rect.left, rect.right);
    rectangle.getVertical(rect.top, rect.bottom);
    return rect;
}


memory::UniquePtr<IWindow> Win32Window::createUnique(const Rectangle<wfl::int32_t>& clientRect)
{
    return WFL_MAKE_UNIQUE(Win32Window, clientRect);
}

memory::SharedPtr<IWindow> Win32Window::createShared(const Rectangle<wfl::int32_t>& clientRect)
{
    return WFL_MAKE_SHARED(Win32Window, clientRect);
}


Win32Window::Win32Window(const Rectangle<wfl::int32_t>& clientRect)
    : m_clientRect(clientRect)
    , m_windowRect(clientRect)
    , m_hInstance(NULL)
    , m_hWindow(NULL)
{
    logging::put("Win32Window()");

    m_hInstance = ::GetModuleHandleA(nullptr);

    if (!m_hInstance) { return; }

    if (!registerWindowClass()) { return; }
    
    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME;

    // 矩形の設定
    if (!adjustWindowRect(style)) { return; }

    if (!createWindow(style)) { return; }
}

Win32Window::~Win32Window()
{
    logging::put("~Win32Window()");

    if (destroyWindow())
    {
        unregisterWindowClass();
    }
}

bool Win32Window::isAlive() const
{
    return ::IsWindow(m_hWindow) == TRUE;
}

bool Win32Window::messagePump()
{
    MSG msg;
    if (::PeekMessageA(&msg, m_hWindow, 0, 0, PM_REMOVE) != 0)
    {
        ::TranslateMessage(&msg);
        ::DispatchMessageA(&msg);
    }

    return msg.message != WM_QUIT;
}


LRESULT CALLBACK Win32Window::windowProcedureEntry(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Win32Window* pInstance = reinterpret_cast<Win32Window*>(::GetWindowLongPtrA(hWnd, GWLP_USERDATA));

    if (pInstance != nullptr)
    {
        return pInstance->windowProcedureBody(
            hWnd,
            uMsg,
            wParam,
            lParam
        );
    }

    return ::DefWindowProcA(
        hWnd,
        uMsg,
        wParam,
        lParam
    );
}

LRESULT CALLBACK Win32Window::windowProcedureBody(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        destroyWindow();
        break;

    case WM_DESTROY:
        m_hWindow = NULL;
        break;
    }

    return ::DefWindowProcA(
        hWnd,
        uMsg,
        wParam,
        lParam
    );
}

bool Win32Window::registerWindowClass()
{
    // 拡張ウィンドウクラスの設定
    WNDCLASSEXA wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = &Win32Window::windowProcedureEntry;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hInstance;
    wc.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hIconSm = LoadIcon(m_hInstance, IDI_APPLICATION);

    if (!::RegisterClassExA(&wc))
    {
        return false;
    }

    return true;
}

bool Win32Window::unregisterWindowClass()
{
    if (m_hInstance != nullptr)
    {
        if (!::UnregisterClassA(WINDOW_CLASS_NAME, m_hInstance))
        {
            assert(false);
            return false;
        }
        m_hInstance = nullptr;
    }
    return true;
}

bool Win32Window::adjustWindowRect(DWORD style)
{
    RECT rect = convertRect(m_clientRect);

    BOOL hasMenu = FALSE;

    // 指定されたクライアント領域を確保するために必要なウィンドウ座標を計算
    if (!::AdjustWindowRect(&rect, style, hasMenu))
    {
        return false;
    }

    m_windowRect = Rectangle<wfl::int32_t>(
        rect.left, rect.right,
        rect.top, rect.bottom
        );

    return true;
}

bool Win32Window::createWindow(DWORD style)
{
    // ウィンドウを生成
    m_hWindow = ::CreateWindowA(
        WINDOW_CLASS_NAME,
        "Win32",
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        m_windowRect.width<int>(),
        m_windowRect.height<int>(),
        nullptr,
        nullptr,
        m_hInstance,
        this
    );

    if (!m_hWindow) { return false; }

    ::SetLastError(0);
    ::ShowWindow(m_hWindow, SW_SHOWNORMAL);
    if (::GetLastError()) { destroyWindow(); return false; }
    
    ::SetLastError(0);
    ::UpdateWindow(m_hWindow);
    if (::GetLastError()) { destroyWindow(); return false; }

    ::SetLastError(0);
    ::SetFocus(m_hWindow);
    if (::GetLastError()) { destroyWindow(); return false; }
    
    ::SetLastError(0);
    ::SetWindowLongPtrA(m_hWindow, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    if (::GetLastError()) { destroyWindow(); return false; }

    return true;
}

bool Win32Window::destroyWindow()
{
    if (m_hWindow)
    {
        if (!::DestroyWindow(m_hWindow)) { return false; }

        m_hWindow = NULL;
    }

    return true;
}


} // namespace application
} // namespace waffle

