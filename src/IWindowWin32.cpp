#include "IWindow.h"
/*
    BSD 2-Clause License

    Copyright (c) 2022, Immanuel Charles
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#if defined(_WIN32)

#include "IWindow.h"

#include <iostream>

namespace IWindow {
    Window::Window(int64_t width, int64_t height, const std::string& title, int64_t x, int64_t y) { Create(width, height, title, x, y); }
    Window::~Window() { 
        ::DestroyIcon(m_icon);
        ::DestroyCursor(m_cursor);
        ::ReleaseDC(m_window, m_deviceContext);
        ::UnregisterClassW(L"IWindow::Window", GetModuleHandle(nullptr));
        ::DestroyWindow(m_window); 
    }

    bool Window::Create(int64_t width, int64_t height, const std::string& title, int64_t x, int64_t y) {
        m_width = width;
        m_height = height;
        m_oldWidth = m_width;
        m_oldHeight = m_height;
        m_x = x;
        m_y = y;
        m_title = title;
        m_userPtr = nullptr;
        m_running = true;
        m_fullscreen = false;
        m_focused = false;
        m_mouseEntered = false;

        m_icon = LoadIcon(nullptr, IDI_APPLICATION);
        m_cursor = LoadCursor(nullptr, IDC_ARROW);

        HINSTANCE instance = GetModuleHandle(nullptr);

        // for multi-window support
        static bool registered = false;

        std::wstring classNane = L"IWindow::Window";

        WNDCLASS wc{};
        wc.lpfnWndProc = s_WindowCallback;
        wc.hInstance = instance;
        wc.lpszClassName = classNane.c_str();
        wc.style = CS_OWNDC;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
        wc.hCursor = m_cursor;
        wc.hIcon = m_icon;
        
        // If already registered dont register again
        if (!registered)
            if (!::RegisterClass(&wc) ) {
                ::MessageBoxA(nullptr, "Failed to register window class!", "Error", MB_ICONEXCLAMATION | MB_OK);
                return false;
            }

        registered = true;

        m_window = 
        ::CreateWindowEx
        (
            WS_EX_APPWINDOW,                                            // Window Type?
            classNane.c_str(),                                    // Class Name
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

        m_timeMS = std::chrono::high_resolution_clock::now();

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
        Window* iWindow = (Window*)GetWindowLongPtr(window, GWLP_USERDATA);

        if (!iWindow) 
            return ::DefWindowProc(window, msg, wparam, lparam);
        

        return iWindow->WindowCallback(window, msg, wparam, lparam);
    }


    bool Window::operator==(IWindow::Window& window) { return m_window == window.GetNativeWindowHandle(); }

    bool Window::operator!=(IWindow::Window& window) { return m_window != window.GetNativeWindowHandle(); }

    LRESULT CALLBACK Window::WindowCallback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam) {
        switch (msg)
        {
        case WM_CLOSE:
            m_running = false;        
            break;
        case WM_MOVE: {
            // if lparam is less than zero than it will act like a uint
            m_x = GET_X_LPARAM(lparam);
            m_y = GET_Y_LPARAM(lparam);
            m_posCallback(*this, m_x, m_y);
            break;
        }
        case WM_MOUSEMOVE: {
            if (!m_mouseEntered) {
                // Tells Win32 to post a msg if the cursor leaves a specific window
                TRACKMOUSEEVENT mouseEvent;
                ::ZeroMemory(&mouseEvent, sizeof(mouseEvent));
                mouseEvent.cbSize = sizeof(mouseEvent);
                mouseEvent.dwFlags = TME_LEAVE;
                mouseEvent.hwndTrack = m_window;
                TrackMouseEvent(&mouseEvent);

                m_mouseEntered = true;
                m_mouseEnteredCallback(*this, m_mouseEntered);
            }
            m_x = GET_X_LPARAM(lparam);
            m_y = GET_Y_LPARAM(lparam);
            m_mouseMovecallback(*this, m_x, m_y);
            break;
        }
        case WM_MOUSELEAVE: {
            m_mouseEntered = false;
            m_mouseEnteredCallback(*this, m_mouseEntered);
            break;
        }
        case WM_SIZE: {
            m_width = LOWORD(lparam);
            m_height = HIWORD(lparam);
            m_sizeCallback(*this, m_width, m_height);
            IVector2 framebufferSize = GetFramebufferSize();
            m_framebufferSizeCallback(*this, framebufferSize.x, framebufferSize.y);
            m_framebufferWidth = framebufferSize.x;
            m_framebufferHeight = framebufferSize.y;
            break;
        }
        case WM_KEYDOWN: {
            m_keys[wparam] = true;
            m_keyCallback(*this, (Key)wparam, InputState::Down);
            break;
        }
        case WM_KEYUP: {
            m_keys[wparam] = false;
            m_keyCallback(*this, (Key)wparam, InputState::Up);
            break;
        }
        case WM_SYSKEYDOWN: {
            m_keys[wparam] = true;
            m_keyCallback(*this, (Key)wparam, InputState::Down);
            break;
        }
        case WM_SYSKEYUP: {
            m_keys[wparam] = false;
            m_keyCallback(*this, (Key)wparam, InputState::Up);
            break;
        }
        case WM_LBUTTONDOWN: {
            m_mouseButtons[(int)MouseButton::Left] = true;
            m_mouseButtonCallback(*this, MouseButton::Left, InputState::Down);
            break;
        }
        case WM_LBUTTONUP: {
            m_mouseButtons[(int)MouseButton::Left] = false;
            m_mouseButtonCallback(*this, MouseButton::Left, InputState::Up);
            break;
        }
        case WM_RBUTTONDOWN: {
            m_mouseButtons[(int)MouseButton::Right] = true;
            m_mouseButtonCallback(*this, MouseButton::Right, InputState::Down);
            break;
        }
        case WM_RBUTTONUP: {
            m_mouseButtons[(int)MouseButton::Right] = false;
            m_mouseButtonCallback(*this, MouseButton::Right, InputState::Up);
            break;
        }
        case WM_MBUTTONDOWN: {
            m_mouseButtons[(int)MouseButton::Middle] = true;
            m_mouseButtonCallback(*this, MouseButton::Middle, InputState::Down);
            break;
        }
        case WM_MBUTTONUP: {
            m_mouseButtons[(int)MouseButton::Middle] = false;
            m_mouseButtonCallback(*this, MouseButton::Middle, InputState::Up);
            break;
        }
        case WM_XBUTTONDOWN: {
            UINT button = GET_XBUTTON_WPARAM(wparam);
            MouseButton sideBtn = button == XBUTTON1 ? MouseButton::Side1 : MouseButton::Side2;
            m_mouseButtons[(int)sideBtn] = true;
            m_mouseButtonCallback(*this, sideBtn, InputState::Down);
            break;
        }
        case WM_XBUTTONUP: {
            UINT button = GET_XBUTTON_WPARAM(wparam);
            MouseButton sideBtn = button == XBUTTON1 ? MouseButton::Side1 : MouseButton::Side2;
            m_mouseButtons[(int)sideBtn] = false;
            m_mouseButtonCallback(*this, sideBtn, InputState::Up);
            break;
        }

        case WM_SETCURSOR: {
            ::SetCursor(m_cursor);
            return true; // Have to return true
        }

        // Y axis
        case WM_MOUSEWHEEL: {
            m_scrollOffsetY = (int16_t)GET_Y_LPARAM(wparam) / (float)WHEEL_DELTA; 
            m_scrollOffsetX = 0.0f;
            m_mouseScrollCallback(*this, m_scrollOffsetX, m_scrollOffsetY);
            break;
        }
        // X axis
        case WM_MOUSEHWHEEL: {
            // X axis inverted for consistency with x11
            m_scrollOffsetX = -((int16_t)GET_Y_LPARAM(wparam) / (float)WHEEL_DELTA);
            m_scrollOffsetY = 0.0f;
            m_mouseScrollCallback(*this, m_scrollOffsetX, m_scrollOffsetY);
            break;
        }
        
        // User focused on the window
        case WM_SETFOCUS: {
            m_focused = true;
            m_windowFocusCallback(*this, m_focused);
            break;
        }

        // User is not focused on the window
        case WM_KILLFOCUS: {
            m_focused = false;
            m_windowFocusCallback(*this, m_focused);
            break;
        }

        case WM_SYSCHAR: 
        case WM_CHAR: {
            wchar_t wch = 0;
            if (::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (char*)&wparam, 1, &wch, 1))
                m_charCallback(*this, wch);
            break;
        }
        

        default:
            break;
        }

        return ::DefWindowProc(window, msg, wparam, lparam);
    }

    IVector2 Window::GetWindowSize() {
        RECT rect;
        ::GetWindowRect(m_window, &rect);

        return IVector2{(int64_t)rect.right - rect.left, (int64_t)rect.bottom - rect.top};
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



    bool Window::IsKeyDown(Key key) { return m_keys[(int64_t)key]; }

    bool Window::IsKeyUp(Key key) { return !IsKeyDown(key); }

    bool Window::IsMouseButtonDown(MouseButton button) { return m_mouseButtons[(int)button]; }
    bool Window::IsMouseButtonUp(MouseButton button) { return !IsMouseButtonDown(button); }


    Vector2 Window::GetMouseScrollOffset() {
        return Vector2{ m_scrollOffsetX, m_scrollOffsetY };
    }

    IVector2 Window::GetWindowPosition() {
        WINDOWPLACEMENT windowPlacement{};
        
        ::GetWindowPlacement(m_window, &windowPlacement);

        return IVector2{(int64_t)windowPlacement.rcNormalPosition.left, (int64_t)windowPlacement.rcNormalPosition.top};
    }

    IVector2 Window::GetFramebufferSize()
    {
        RECT rect;
        ::GetClientRect(m_window, &rect);

        return IVector2{ (int64_t)rect.right - rect.left, (int64_t)rect.bottom - rect.top };
    }

    void Window::SetUserPointer(void* ptr) { m_userPtr = ptr; }

    void* Window::GetUserPointer() { return m_userPtr; }

    Window::WindowPosCallback Window::SetPosCallback(WindowPosCallback callback) { 
        WindowPosCallback oldCallback = m_posCallback;
        m_posCallback = callback;
        return oldCallback;
    }

    Window::WindowSizeCallback Window::SetSizeCallback(WindowSizeCallback callback) { 
        WindowPosCallback oldCallback = m_sizeCallback;
        m_sizeCallback = callback;
        return oldCallback;
    }

    Window::KeyCallback Window::SetKeyCallback(KeyCallback callback) { 
        KeyCallback oldCallback = m_keyCallback;
        m_keyCallback = callback;
        return oldCallback;
    }

    Window::MouseMoveCallback Window::SetMouseMoveCallback(MouseMoveCallback callback) { 
        MouseMoveCallback oldCallback = m_mouseMovecallback;
        m_mouseMovecallback = callback;
        return oldCallback;
    }

    Window::MouseButtonCallback Window::SetMouseButtonCallback(MouseButtonCallback callback) { 
        MouseButtonCallback oldCallback = m_mouseButtonCallback;
        m_mouseButtonCallback = callback;
        return oldCallback;
    }

    Window::MouseScrollCallback Window::SetMouseScrollCallback(MouseScrollCallback callback) { 
        MouseScrollCallback oldCallback = m_mouseScrollCallback;
        m_mouseScrollCallback = callback;
        return oldCallback;
    }

    Window::WindowFocusCallback Window::SetWindowFocusCallback(WindowFocusCallback callback)
    {
        WindowFocusCallback oldCallback = m_windowFocusCallback;
        m_windowFocusCallback = callback;
        return oldCallback;
    }

    Window::MouseEnteredCallback Window::SetMouseEnteredCallback(MouseEnteredCallback callback)
    {
        MouseEnteredCallback oldCallback = m_mouseEnteredCallback;
        m_mouseEnteredCallback = callback;
        return oldCallback;
    }

    Window::CharCallback Window::SetCharCallback(CharCallback callback)
    {
        CharCallback oldcallback = m_charCallback;
        m_charCallback = callback;
        return oldcallback;
    }

    Window::FramebufferSizeCallback Window::SetFramebufferSizeCallback(FramebufferSizeCallback callback)
    {
        FramebufferSizeCallback oldcallback = m_framebufferSizeCallback;
        m_framebufferSizeCallback = callback;
        return oldcallback;
    }

    IVector2 Window::GetMousePosition() { return IVector2{ m_mouseX, m_mouseY }; }

    void Window::SetMousePos(int64_t x, int64_t y) { ::SetCursorPos(x, y); }

    Monitor Window::GetPrimaryMonitor() {
        // The primary montitors top left corner is always 0, 0
        const POINT p = {0, 0};
        HMONITOR hmonitor = ::MonitorFromPoint(p, MONITOR_DEFAULTTOPRIMARY);

        MONITORINFOEX monitorInfo{}; // EX has the monitor name
        monitorInfo.cbSize = sizeof(MONITORINFOEX);
        if (!::GetMonitorInfo(hmonitor, &monitorInfo)) { 
            MessageBox(nullptr, L"Failed to get primary monitor!", L"Error", MB_ICONEXCLAMATION | MB_OK);
            return Monitor{}; 
        }

        Monitor monitor{};

        monitor.size.x = (int64_t)monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left; 
        monitor.size.y = (int64_t)monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
        monitor.position.x = (int64_t)monitorInfo.rcMonitor.left;
        monitor.position.y = (int64_t)monitorInfo.rcMonitor.top;
        monitor.name = monitorInfo.szDevice;

        return monitor;
    }



    BOOL CALLBACK MonitorCallback(HMONITOR hmonitor, HDC, LPRECT rc, LPARAM lparam) {
        std::vector<Monitor>* monitorsVec = (std::vector<Monitor>*)lparam;

        MONITORINFOEX monitorInfo{}; // EX has the monitor name
        monitorInfo.cbSize = sizeof(MONITORINFOEX);
        if (!::GetMonitorInfo(hmonitor, &monitorInfo)) {
            MessageBox(nullptr, L"Failed to get monitors!", L"Error", MB_ICONEXCLAMATION | MB_OK);
            return false;
        }

        Monitor monitor{};

        monitor.size.x = (int64_t)monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
        monitor.size.y = (int64_t)monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
        monitor.position.x = (int64_t)monitorInfo.rcMonitor.left;
        monitor.position.y = (int64_t)monitorInfo.rcMonitor.top;
        monitor.name = monitorInfo.szDevice;


        monitorsVec->emplace_back(std::move(monitor));

        return true;
    }

    std::vector<Monitor> Window::GetAllMonitors() {
        std::vector<Monitor> monitors;
        ::EnumDisplayMonitors(nullptr, nullptr, MonitorCallback, (LPARAM)&monitors);
        return monitors;
    }

    void Window::Center(Monitor monitor) {
        SetWindowPosition( ( monitor.size.x - m_width ) / 2 , ( monitor.size.y - m_height ) / 2);
    }

    void Window::Fullscreen(bool fullscreen, Monitor monitor) {
        if (m_fullscreen == fullscreen)
            return;

        m_fullscreen = fullscreen;

        // No fullscreen
        if (!m_fullscreen) {
            SetWindowLongPtr(m_window, GWL_EXSTYLE, WS_EX_LEFT);
            SetWindowLongPtr(m_window, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
            SetWindowSize(m_oldWidth, m_oldHeight);
            Center(monitor);
            return;
        }

        // Fullscreen
        m_oldWidth = m_width;
        m_oldHeight = m_height;

        SetWindowLongPtr(m_window, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
        SetWindowLongPtr(m_window, GWL_STYLE, WS_POPUP | WS_VISIBLE);

        ::SetWindowPos(m_window, nullptr,
            (int)monitor.position.x, (int)monitor.position.y,
            (int)monitor.size.x, (int)monitor.size.y,
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }

    bool Window::IsFullscreen() { return m_fullscreen; }

    // Orignally was glfw3's implementation reworked to work with IWindow
    HANDLE CreateImage(Image image, uint32_t hotX, uint32_t hotY, bool isIcon) {
        HANDLE imageHandle;
        HBITMAP colour, mask;
        BITMAPV5HEADER bi;
        ICONINFO ii;
        uint8_t* target = NULL;
        uint8_t* source = image.data;

        ::ZeroMemory(&bi, sizeof(bi));
        bi.bV5Size = sizeof(bi);
        bi.bV5Width = image.width;
        // Doing image.height will make the image upside down
        bi.bV5Height = -image.height;
        bi.bV5Planes = 1;
        bi.bV5BitCount = 32;
        bi.bV5Compression = BI_BITFIELDS;
        bi.bV5RedMask = 0x00ff0000;
        bi.bV5GreenMask = 0x0000ff00;
        bi.bV5BlueMask = 0x000000ff;
        bi.bV5AlphaMask = 0xff000000;

        HDC deviceContext = ::GetDC(nullptr);

        colour = ::CreateDIBSection(deviceContext,
            (BITMAPINFO*)&bi,
            DIB_RGB_COLORS,
            (void**)&target,
            NULL,
            (DWORD)0);


        mask = ::CreateBitmap(image.width, image.height, 1, 1, NULL);

        ::ReleaseDC(nullptr, deviceContext);


        for (int32_t i = 0; i < image.width * image.height; i++) {
            target[0] = source[2];
            target[1] = source[1];
            target[2] = source[0];
            target[3] = source[3];
            target += 4;
            source += 4;
        }

        ::ZeroMemory(&ii, sizeof(ii));
        ii.fIcon = isIcon;
        ii.xHotspot = hotX;
        ii.yHotspot = hotY;
        ii.hbmMask = mask;
        ii.hbmColor = colour;

        imageHandle = ::CreateIconIndirect(&ii);
        if (colour)
            ::DeleteObject(colour);
        if (mask)
            ::DeleteObject(mask);

        return imageHandle;
    }

    void Window::SetIcon(Image image) {
        m_icon = (NativeIcon)CreateImage(image, 0, 0, true);

        ::SendMessage(m_window, WM_SETICON, ICON_SMALL, (LPARAM)m_icon);
        ::SendMessage(m_window, WM_SETICON, ICON_BIG, (LPARAM)m_icon);
    }

    void Window::SetCursor(Image image, uint32_t hotX, uint32_t hotY) {
        m_cursor = (NativeCursor)CreateImage(image, hotX, hotY, false);
        ::SendMessage(m_window, WM_SETCURSOR, (WPARAM)m_window, (LPARAM)0);
    }

    void Window::SetIcon(NativeIconID iconID) {
        m_icon = ::LoadCursor(nullptr, MAKEINTRESOURCE(iconID));
        ::SendMessage(m_window, WM_SETICON, ICON_SMALL, (LPARAM)m_icon);
        ::SendMessage(m_window, WM_SETICON, ICON_BIG, (LPARAM)m_icon);
    }

    void Window::SetCursor(NativeCursorID cursorID) {
        m_cursor = ::LoadCursor(nullptr, MAKEINTRESOURCE(cursorID));
        ::SendMessage(m_window, WM_SETCURSOR, (WPARAM)m_window, (LPARAM)0);
    }

    std::string Window::GetClipboardText()
    {
        if (!::OpenClipboard(nullptr)) return "";

        // CF_TEXT = ASCII Text
        HANDLE data = ::GetClipboardData(CF_TEXT);
        // Lock the data so we cant ctrl-c a new string while copying the current string
        const char* c_data  = (char*)::GlobalLock(data);
        if (!c_data) return "";

        ::GlobalUnlock(data);

        ::CloseClipboard();

        return c_data;
    }

    void Window::SetClipboardText(const std::string& text)
    {

        HGLOBAL mem = ::GlobalAlloc(GMEM_MOVEABLE, text.length());
        memcpy(::GlobalLock(mem), text.c_str(), text.length());
        ::GlobalUnlock(mem);

        if (!::OpenClipboard(nullptr)) {
            std::cout << "Cant set clipboard text!\n";
            return;
        }

        ::EmptyClipboard();
        ::SetClipboardData(CF_TEXT, mem);

        ::CloseClipboard();
    }

    NativeDeviceContext& Window::GetNativeDeviceContext() { return m_deviceContext; }
    double Window::GetTime() { 
        std::chrono::duration<double, std::milli> dur = std::chrono::high_resolution_clock::now() - m_timeMS;
        return dur.count(); 
    }
    bool Window::IsFocused() { return m_focused; }


    void IWindow::Window::SetTitle(const std::string& title) {  SetWindowTextA(m_window, (LPCSTR)title.c_str()); }
}

#endif