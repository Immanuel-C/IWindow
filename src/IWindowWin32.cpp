#include "IWindow.h"

#include <iostream>

namespace IWindow {
    Window::Window(int64_t width, int64_t height, const std::string& title, int64_t x, int64_t y) { Create(width, height, title, x, y); }
    Window::~Window() { 
        ::ReleaseDC(m_window, m_deviceContext);
        ::UnregisterClassW(TEXT("IWindow::Window"), GetModuleHandle(nullptr));
        ::DestroyWindow(m_window); 
    }

    bool Window::Create(int64_t width, int64_t height, const std::string& title, int64_t x, int64_t y) {
        m_width = width;
        m_height = height;
        m_oldWidth = width;
        m_oldHeight = m_height;
        m_x = x;
        m_y = y;
        m_title = title;
        
        HINSTANCE instance = GetModuleHandle(nullptr);


        WNDCLASS wc{};
        wc.lpfnWndProc = s_WindowCallback;
        wc.hInstance = instance;
        wc.lpszClassName = TEXT("IWindow::Window");
        wc.style = CS_OWNDC | CS_DBLCLKS ;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);


        if (!::RegisterClass(&wc)) {
            ::MessageBoxA(nullptr, "Failed to register window class!", "Error", MB_ICONEXCLAMATION | MB_OK);
            return false;
        }

        m_window = 
        ::CreateWindowEx
        (
            WS_EX_APPWINDOW,                                            // Window Type?
            TEXT("IWindow::Window"),                                    // Class Name
            std::wstring{title.begin(), title.end()}.c_str(),           // Window Name
            WS_OVERLAPPEDWINDOW |                 
            WS_CAPTION          | 
            WS_SYSMENU          | 
            WS_MINIMIZEBOX      | 
            WS_MAXIMIZEBOX      | 
            WS_OVERLAPPED,                                              // Styles
            (int)x,                                                     // PosX
            (int)y,                                                     // PosY
            (int)width,                                                 // SizeX
            (int)height,                                                // SizeY
            nullptr,                                                    // Parent Window
            nullptr,                                                    // HMENU?
            instance,                                                   // hinstance
            nullptr                                                     // lpParam LPVOID?
        );

        if (!m_window) {
            ::MessageBoxA(nullptr, "Failed creating window!", "Error", MB_ICONEXCLAMATION | MB_OK);
            return false; 
        }

        ::SetWindowLongPtr(m_window, GWLP_USERDATA, (LONG_PTR)this);

        ::ShowWindow(m_window, SW_SHOW);

        m_deviceContext = GetDC(m_window);

        return true;
    }

    void Window::Update() {
        MSG msg;

        // Get all messages
        while (::PeekMessage(&msg, m_window, 0, 0, PM_REMOVE)) {
            // ?
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        } 
    }

    bool Window::IsRunning() { return m_running; }

    NativeWindowHandle& Window::GetNativeWindowHandle() { return m_window; };


    LRESULT CALLBACK Window::s_WindowCallback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam) {
        static Window* iWindow = (Window*)GetWindowLongPtr(window, GWLP_USERDATA);

        if (!iWindow) {
            iWindow = (Window*)GetWindowLongPtr(window, GWLP_USERDATA);
            return ::DefWindowProc(window, msg, wparam, lparam);
        }

        return iWindow->WindowCallback(window, msg, wparam, lparam);
    }


    LRESULT CALLBACK Window::WindowCallback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam) {
        switch (msg)
        {
        case WM_CLOSE:
            m_running = false;        
            break;
        case WM_MOVE: {
            // if lparam is less than zero than it will act like a uint
            m_x = LOWORD(lparam) >= 65000 ? 0 : LOWORD(lparam);
            m_y = HIWORD(lparam) >= 65000  ? 0 : HIWORD(lparam);
            m_posCallback(*this, m_x, m_y);
            break;
        }
        case WM_SIZE: {
            m_width = LOWORD(lparam);
            m_height = HIWORD(lparam);
            m_sizeCallback(*this, m_width, m_height);
            break;
        }
        case WM_KEYDOWN: 
            m_keys[wparam] = true;
            m_keyCallback(*this, (Key)wparam, InputState::Down);
            break;
        case WM_KEYUP: 
            m_keys[wparam] = false;
            m_keyCallback(*this, (Key)wparam, InputState::Up);
            break;
        case WM_SYSKEYDOWN:
            m_keys[wparam] = true;
            m_keyCallback(*this, (Key)wparam, InputState::Down);
            break;
        case WM_SYSKEYUP:
            m_keys[wparam] = false;
            m_keyCallback(*this, (Key)wparam, InputState::Up);
            break;
        case WM_MOUSEMOVE:
            m_mouseX =  GET_X_LPARAM(lparam);
            m_mouseY =  GET_Y_LPARAM(lparam);
            m_mouseMovecallback(*this, m_mouseX, m_mouseY);
            break;


        case WM_LBUTTONDOWN:
            m_mouseButtons[(int)MouseButton::Left] = true;
            m_mouseButtonCallback(*this, MouseButton::Left, InputState::Down, ClickState::Single);
            break;
        case WM_LBUTTONUP:
            m_mouseButtons[(int)MouseButton::Left] = false;
            m_mouseButtonsDbl[(int)MouseButton::Left] = false;
            m_mouseButtonCallback(*this, MouseButton::Left, InputState::Up, ClickState::Up);
            break;
        case WM_LBUTTONDBLCLK:
            m_mouseButtonsDbl[(int)MouseButton::Left] = true;
            m_mouseButtonCallback(*this, MouseButton::Left, InputState::Down, ClickState::Double);
            break;

        case WM_RBUTTONDOWN:
            m_mouseButtons[(int)MouseButton::Right] = true;
            m_mouseButtonCallback(*this, MouseButton::Right, InputState::Down, ClickState::Single);
            break;
        case WM_RBUTTONUP:
            m_mouseButtons[(int)MouseButton::Right] = false;
            m_mouseButtonsDbl[(int)MouseButton::Right] = false;
            m_mouseButtonCallback(*this, MouseButton::Right, InputState::Up, ClickState::Up);
            break;
        case WM_RBUTTONDBLCLK:
            m_mouseButtons[(int)MouseButton::Right] = true;
            m_mouseButtonsDbl[(int)MouseButton::Right] = true;
            m_mouseButtonCallback(*this, MouseButton::Right, InputState::Down, ClickState::Double);
            break;


        case WM_MBUTTONDOWN:
            m_mouseButtons[(int)MouseButton::Middle] = true;
            m_mouseButtonCallback(*this, MouseButton::Middle, InputState::Down, ClickState::Single);
            break;
        case WM_MBUTTONUP:
            m_mouseButtons[(int)MouseButton::Middle] = false;
            m_mouseButtonsDbl[(int)MouseButton::Middle] = false;
            m_mouseButtonCallback(*this, MouseButton::Middle, InputState::Up, ClickState::Up);
            break;
        case WM_MBUTTONDBLCLK:
            m_mouseButtonsDbl[(int)MouseButton::Middle] = true;
            m_mouseButtonCallback(*this, MouseButton::Middle, InputState::Down, ClickState::Double);
            break;


        case WM_XBUTTONDOWN: {
            UINT button = GET_XBUTTON_WPARAM(wparam);
            MouseButton sideBtn = button == XBUTTON1 ? MouseButton::Side1 : MouseButton::Side2;
            m_mouseButtons[(int)sideBtn] = true;
            m_mouseButtonCallback(*this, sideBtn, InputState::Down, ClickState::Single);
        }
        case WM_XBUTTONUP: {
            UINT button = GET_XBUTTON_WPARAM(wparam);
            MouseButton sideBtn = button == XBUTTON1 ? MouseButton::Side1 : MouseButton::Side2;
            m_mouseButtons[(int)sideBtn] = false;
            m_mouseButtonsDbl[(int)sideBtn] = false;
            m_mouseButtonCallback(*this, sideBtn, InputState::Up, ClickState::Up);
        }
        case WM_XBUTTONDBLCLK: {
            UINT button = GET_XBUTTON_WPARAM(wparam);
            MouseButton sideBtn = button == XBUTTON1 ? MouseButton::Side1 : MouseButton::Side2;
            m_mouseButtonsDbl[(int)sideBtn] = true;
            m_mouseButtonCallback(*this, sideBtn, InputState::Down, ClickState::Double);
            break;
        }
        default:
            break;
        }

        return ::DefWindowProc(window, msg, wparam, lparam);
    }

    Vector2 Window::GetWindowSize() {
        RECT rect;
        ::GetClientRect(m_window, &rect);

        return Vector2{(int64_t)rect.right - rect.left, (int64_t)rect.bottom - rect.top};
    }

    void Window::SetWindowSize(int64_t width, int64_t height) {
        m_width = width;
        m_height = height;

        ::SetWindowPos(m_window, nullptr, (int)m_x, (int)m_y, (int)m_width, (int)m_height, SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
    }

    void Window::SetWindowPosition(int64_t x, int64_t y) {
        m_x = x;
        m_y = y;

        ::SetWindowPos(m_window, nullptr, (int)m_x, (int)m_y, (int)m_width, (int)m_height, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
    }

    bool Window::IsKeyDown(Key key) { return m_keys[(int)key]; }

    bool Window::IsKeyUp(Key key) { return !IsKeyDown(key); }

    bool Window::IsMouseButtonDown(MouseButton button) { return m_mouseButtons[(int)button]; }
    bool Window::IsMouseButtonDoubleClicked(MouseButton button) { return m_mouseButtonsDbl[(int)button]; }
    bool Window::IsMouseButtonUp(MouseButton button) { return !IsMouseButtonDown(button) || IsMouseButtonDoubleClicked(button); }


    Vector2 Window::GetWindowPosition() {
        WINDOWPLACEMENT windowPlacement{};
        
        ::GetWindowPlacement(m_window, &windowPlacement);

        return Vector2{(int64_t)windowPlacement.rcNormalPosition.left, (int64_t)windowPlacement.rcNormalPosition.top};
    }

    void Window::SetUserPointer(void* ptr) { m_userPtr = ptr; }

    void* Window::GetUserPointer() { return m_userPtr; }

    void Window::SetPosCallback(WindowPosCallback callback) { m_posCallback = callback; }
    void Window::SetSizeCallback(WindowSizeCallback callback) { m_sizeCallback = callback; }
    void Window::SetKeyCallback(KeyCallback callback) { m_keyCallback = callback; }
    void Window::SetMouseMoveCallback(MouseMoveCallback callback) { m_mouseMovecallback = callback; }
    void Window::SetMouseButtonCallback(MouseButtonCallback callback) { m_mouseButtonCallback = callback; }


    Vector2 Window::GetMousePosition() { return Vector2{ m_mouseX, m_mouseY }; }

    void Window::Center() {
        MONITORINFO mi{};
        mi.cbSize = sizeof(MONITORINFO);
        
        if (!::GetMonitorInfo(::MonitorFromWindow(m_window, MONITOR_DEFAULTTONEAREST), &mi)) return;

        SetWindowPosition( ( mi.rcMonitor.right - m_width ) / 2 , ( mi.rcMonitor.bottom - m_height ) / 2);
    }

    void Window::Fullscreen(bool fullscreen) {
        if (m_fullscreen == fullscreen)
            return;

        m_fullscreen = fullscreen;

        // No fullscreen
        if (!m_fullscreen) {
            SetWindowLongPtr(m_window, GWL_EXSTYLE, WS_EX_LEFT);
            SetWindowLongPtr(m_window, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
            SetWindowSize(m_oldWidth, m_oldHeight);
            Center();
            return;
        }

        // Fullscreen
        m_oldWidth = m_width;
        m_oldHeight = m_height;

        MONITORINFO mi = { sizeof(mi) };
        if (!::GetMonitorInfo(MonitorFromWindow(m_window, MONITOR_DEFAULTTOPRIMARY), &mi)) return;

        SetWindowLongPtr(m_window, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
        SetWindowLongPtr(m_window, GWL_STYLE, WS_POPUP | WS_VISIBLE);

        ::SetWindowPos(m_window, nullptr,
            mi.rcMonitor.left, mi.rcMonitor.top,
            mi.rcMonitor.right - mi.rcMonitor.left,
            mi.rcMonitor.bottom - mi.rcMonitor.top,
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }

    bool Window::IsFullscreen() { return m_fullscreen; }

    NativeGLDeviceContext& Window::GetNativeGLDeviceContext() { return m_deviceContext; }
}