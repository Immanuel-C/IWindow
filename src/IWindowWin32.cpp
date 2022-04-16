#include "IWindow.h"


#include <iostream>

namespace IWindow {
    Window::Window(int64_t width, int64_t height, const std::string& title, int64_t x, int64_t y) { Create(width, height, title, x, y); }
    Window::~Window() { ::DestroyWindow(m_window); }

    bool Window::Create(int64_t width, int64_t height, const std::string& title, int64_t x, int64_t y) {
        m_width = width;
        m_height = height;
        m_x = x;
        m_y = y;
        m_title = title;
        
        m_posCallback = DefaultWindowPosCallback;
        m_sizeCallback = DefaultWindowSizeCallback;

        
        HINSTANCE instance = GetModuleHandleA(0);

        WNDCLASS wc{};
        wc.lpfnWndProc = s_WindowCallback;
        wc.hInstance = instance;
        wc.lpszClassName = TEXT("IWindow::Window");
        wc.style = CS_OWNDC;
        wc.hbrBackground = (HBRUSH)(COLOR_ACTIVEBORDER);


        if (!::RegisterClass(&wc)) {
            ::MessageBoxA(nullptr, "Failed to register window class!", "Error", MB_ICONEXCLAMATION | MB_OK);
            return false;
        }

        m_window = 
        ::CreateWindowExW
        (
            WS_EX_APPWINDOW,                       // Window Type?
            TEXT("IWindow::Window"),               // Class Name
            std::wstring{title.begin(), title.end()}.c_str(),           // Window Name
            WS_OVERLAPPEDWINDOW |                 
            WS_CAPTION          | 
            WS_SYSMENU          | 
            WS_MINIMIZEBOX      | 
            WS_MAXIMIZEBOX      | 
            WS_OVERLAPPED,                          // Styles
            (int)x,                                 // PosX
            (int)y,                                 // PosY
            (int)width,                             // SizeX
            (int)height,                            // SizeY
            nullptr,                                // Parent Window
            nullptr,                                // HMENU?
            instance,                               // hinstance
            nullptr                                 // lpParam LPVOID?
        );

        if (!m_window) {
            ::MessageBoxA(nullptr, "Failed creating window!", "Error", MB_ICONEXCLAMATION | MB_OK);
            return false; 
        }

        ::SetWindowLongPtr(m_window, GWLP_USERDATA, (LONG_PTR)this);

        ::ShowWindow(m_window, SW_SHOW);

        return true;
    }

    void Window::Update() {
        MSG msg;

        // Get all messages
        while (::PeekMessageA(&msg, m_window, 0, 0, PM_REMOVE)) {
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
            if (::IsWindowUnicode(window))
                return ::DefWindowProcW(window, msg, wparam, lparam);
            return ::DefWindowProcA(window, msg, wparam, lparam);
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
            break;
        case WM_KEYUP: 
            m_keys[wparam] = false;
            break;
        case WM_SYSKEYDOWN:
            m_keys[wparam] = true;
            break;
        case WM_SYSKEYUP:
            m_keys[wparam] = false;
            break;
        default:
            break;
        }

        if (::IsWindowUnicode(window))
            return ::DefWindowProcW(window, msg, wparam, lparam);
        
        return ::DefWindowProcA(window, msg, wparam, lparam);
    }

    WindowSize Window::GetWindowSize() {
        RECT rect;
        ::GetClientRect(m_window, &rect);

        return WindowSize{(int64_t)rect.right - rect.left, (int64_t)rect.bottom - rect.top};
    }

    void Window::SetWindowSize(int64_t width, int64_t height) {
        m_width = width;
        m_height = height;

        ::SetWindowPos(m_window, nullptr, (int)m_x, (int)m_y, (int)m_width, (int)m_height, SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
    }

    void Window::SetWindowPosiiton(int64_t x, int64_t y) {
        m_x = x;
        m_y = y;

        ::SetWindowPos(m_window, nullptr, (int)m_x, (int)m_y, (int)m_width, (int)m_height, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
    }

    bool Window::IsKeyDown(Key key) { return m_keys[(int)key]; }

    bool Window::IsKeyUp(Key key) { return !IsKeyDown(key); }



    WindowPos Window::GetWindowPosition() {
        WINDOWPLACEMENT windowPlacement{};
        
        ::GetWindowPlacement(m_window, &windowPlacement);

        return WindowPos{(int64_t)windowPlacement.rcNormalPosition.left, (int64_t)windowPlacement.rcNormalPosition.top};
    }


    void Window::SetUserPointer(void* ptr) { m_userPtr = ptr; }

    void* Window::GetUserPointer() { return m_userPtr; }

    void Window::SetPosCallback(WindowPosCallback callback) { m_posCallback = callback; }

    void Window::SetSizeCallback(WindowSizeCallback callback) { m_sizeCallback = callback; }
}