#include "win32_window.h"


namespace waffle {
namespace application {

constexpr const char* WINDOW_CLASS_NAME = "WAFFLE_WINDOW";

static RECT convertRect(const Rectangle<wfl::int32_t>& rectangle)
{
    RECT result = {};
    rectangle.getHorizontal(result.left, result.right);
    rectangle.getVertical(result.top, result.bottom);
    return result;
}

static Rectangle<wfl::int32_t> convertRect(const RECT& rect)
{
    Rectangle<wfl::int32_t> result(
        rect.left, rect.right,
        rect.top, rect.bottom
    );
    return result;
}

static HICON loadIcon(const char* iconName, HINSTANCE hInstance, wfl::int32_t iconSize)
{
    HICON result = reinterpret_cast<HICON>(
        ::LoadImageA(
            hInstance,
            "WAFFLE_ICON",
            IMAGE_ICON,
            static_cast<int>(iconSize),
            static_cast<int>(iconSize),
            LR_DEFAULTSIZE | LR_SHARED));
    return result;
}

bool getClassInfo(WNDCLASSEXA& windowClassEX, HINSTANCE hInstance)
{
    bool result = ::GetClassInfoExA(hInstance, WINDOW_CLASS_NAME, &windowClassEX) != 0;
    return result;
}

bool Win32Window::createUnique(
    const Rectangle<wfl::int32_t>& clientRect,
    UniquePtr<IWindow>& outWindow)
{
    UniquePtr<Win32Window> ptr = WFL_MAKE_UNIQUE(Win32Window);

    if (ptr->initialize(clientRect))
    {
        outWindow = wfl::move(ptr);
        return true;
    }
    
    return false;
}

bool Win32Window::createShared(
    const Rectangle<wfl::int32_t>& clientRect,
    SharedPtr<IWindow>& outWindow)
{
    SharedPtr<Win32Window> ptr = WFL_MAKE_SHARED(Win32Window);

    if (ptr->initialize(clientRect))
    {
        outWindow = wfl::move(ptr);
        return true;
    }

    return false;
}

Win32Window::Win32Window()
    : m_clientRect()
    , m_windowRect()
    , m_hInstance(NULL)
    , m_hWindow(NULL)
    , m_style(0)
{}

Win32Window::~Win32Window()
{
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

bool Win32Window::setTitle(const StringView& title)
{
    if (!isAlive()) { return false; }

    LPARAM lParam = reinterpret_cast<LPARAM>(title.data());
    LRESULT result = ::SendMessageA(m_hWindow, WM_SETTEXT, 0, lParam);

    return SUCCEEDED(result);
}

bool Win32Window::setClientRect(const Rectangle<wfl::int32_t>& clientRect)
{
    if (!isAlive()) { return false; }

    const Rectangle<wfl::int32_t> clientRectOld = m_clientRect;
    const Rectangle<wfl::int32_t> windowRectOld = m_windowRect;

    m_clientRect = clientRect;

    if (!adjustWindowRect(m_style))
    {
        m_clientRect = clientRectOld;
        m_windowRect = windowRectOld;
        return false;
    }

    RECT rect = convertRect(m_clientRect);

    ::SetWindowPos(
        m_hWindow,
        NULL,
        rect.left, rect.top,
        m_windowRect.width<int>(), m_windowRect.height<int>(),
        SWP_NOMOVE | SWP_NOZORDER);

    updateRectangles();

    return true;
}

void* Win32Window::windowHandle() const
{
    return reinterpret_cast<void*>(m_hWindow);
}

void* Win32Window::applicationHandle() const
{
    return reinterpret_cast<void*>(m_hInstance);
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

    case WM_SIZE:
        updateRectangles();
        break;
    }

    return ::DefWindowProcA(
        hWnd,
        uMsg,
        wParam,
        lParam
    );
}

bool Win32Window::initialize(const Rectangle<wfl::int32_t>& clientRect)
{
    m_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME;

    m_clientRect = clientRect;
    m_windowRect = clientRect;

    m_hInstance = ::GetModuleHandleA(nullptr);

    if (!m_hInstance) { return false; }

    if (!registerWindowClass()) { return false; }

    // 矩形の設定
    if (!adjustWindowRect(m_style)) { return false; }

    if (!createWindow(m_style)) { return false; }

    return true;
}

bool Win32Window::registerWindowClass()
{
    constexpr wfl::int32_t largeIconSize = 64;
    constexpr wfl::int32_t smallIconSize = 16;

    // 拡張ウィンドウクラスの設定
    WNDCLASSEXA windowClassEX = {};

    if (!getClassInfo(windowClassEX, m_hInstance))
    {
        windowClassEX.cbSize = sizeof(WNDCLASSEX);
        windowClassEX.style = CS_HREDRAW | CS_VREDRAW;
        windowClassEX.lpfnWndProc = &Win32Window::windowProcedureEntry;
        windowClassEX.cbClsExtra = 0;
        windowClassEX.cbWndExtra = 0;
        windowClassEX.hInstance = m_hInstance;
        windowClassEX.hIcon = loadIcon("WAFFLE_ICON", m_hInstance, largeIconSize);
        windowClassEX.hIconSm = loadIcon("WAFFLE_ICON", m_hInstance, smallIconSize);
        windowClassEX.hCursor = LoadCursor(nullptr, IDC_ARROW);
        windowClassEX.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        windowClassEX.lpszMenuName = nullptr;
        windowClassEX.lpszClassName = WINDOW_CLASS_NAME;
    }

    if (!::RegisterClassExA(&windowClassEX))
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

bool Win32Window::updateRectangles()
{
    return updateClientRect() && updateWindowRect();
}

bool Win32Window::updateClientRect()
{
    if (!isAlive()) { return false; }

    RECT clientRect = {};
    if (!::GetClientRect(m_hWindow, &clientRect))
    {
        return false;
    }

    m_clientRect = convertRect(clientRect);

    return true;
}

bool Win32Window::updateWindowRect()
{
    if (!isAlive()) { return false; }

    RECT windowRect = {};
    if (!::GetWindowRect(m_hWindow, &windowRect))
    {
        return false;
    }

    m_windowRect = convertRect(windowRect);

    return true;
}


} // namespace application
} // namespace waffle

