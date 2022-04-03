#include "IWindow.h"


#include <iostream>

namespace IWindow {
    void DefaultWindowPosCallback(Window& window, uint32_t x, uint32_t y) { 
    }

    Window::Window(uint32_t width, uint32_t height, const std::string& title, uint32_t x, uint32_t y) { Create(width, height, title, x, y); }

    bool Window::Create(uint32_t width, uint32_t height, const std::string& title, uint32_t x, uint32_t y) {
        m_width = width;
        m_height = height;
        m_x = x;
        m_y = y;
        
        m_posCallback = DefaultWindowPosCallback;

        
        HINSTANCE instance = GetModuleHandleA(0);

        WNDCLASS wc{};
        wc.lpfnWndProc = s_WindowCallback;
        wc.hInstance = instance;
        wc.lpszClassName = TEXT("Window");

        if (!::RegisterClass(&wc)) {
            ::MessageBoxA(nullptr, "Failed to register window class!", "Error", MB_ICONEXCLAMATION | MB_OK);
            return false;
        }

        m_window = 
        ::CreateWindowExA
        (
            WS_EX_APPWINDOW,        // Window Type?
            "Window",               // Class Name
            title.c_str(),          // Window Name
            WS_OVERLAPPEDWINDOW |                 
            WS_CAPTION          | 
            WS_SYSMENU          | 
            WS_MINIMIZEBOX      | 
            WS_MAXIMIZEBOX      | 
            WS_OVERLAPPED,          // Styles
            x,                      // PosX
            y,                      // PosY
            width,                  // SizeX
            height,                 // SizeY
            nullptr,                // Parent Window
            nullptr,                // ? HMENU
            instance,               // hinstance
            this                    // user param
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
            return DefWindowProcA(window, msg, wparam, lparam);
        }

        return iWindow->WindowCallback(window, msg, wparam, lparam);
    }

    LRESULT CALLBACK Window::WindowCallback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam) {
        switch (msg)
        {
        case WM_CLOSE:
            m_running = false;        
            break;
        case WM_WINDOWPOSCHANGING:
            SetWindowPosiiton(GetWindowPosition().x, GetWindowPosition().y);
            m_posCallback(*this, GetWindowPosition().x, GetWindowPosition().y);
            break;
        default:
            break;
        }
        
        return DefWindowProcA(window, msg, wparam, lparam);
    }

    WindowSize Window::GetWindowSize() {
        RECT rect;
        ::GetClientRect(m_window, &rect);

        return WindowSize{(uint32_t)rect.right - rect.left, (uint32_t)rect.bottom - rect.top};
    }

    void Window::SetWindowSize(uint32_t width, uint32_t height) {
        m_width = width;
        m_height = height;

        ::SetWindowPos(m_window, nullptr, m_x, m_y, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);    
    }

    void Window::SetWindowPosiiton(uint32_t x, uint32_t y) {
        m_x = x;
        m_y = y;

        ::SetWindowPos(m_window, nullptr, m_x, m_y, m_width, m_height, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);    
    }

    Window::~Window() { ::DestroyWindow(m_window); }

    WindowPos Window::GetWindowPosition() {
        WINDOWPLACEMENT windowPlacement;
        
        ::GetWindowPlacement(m_window, &windowPlacement);

        return WindowPos{(uint32_t)windowPlacement.rcNormalPosition.left, (uint32_t)windowPlacement.rcNormalPosition.top};
    }


    void Window::SetPosCallback(WindowPosCallback callback) {
        m_posCallback = callback;
    }
}