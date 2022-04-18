#include "IWindow.h"

#include <iostream>

namespace IWindow {
    Window::Window(int64_t width, int64_t height, const std::string& title, int64_t x, int64_t y) { Create(width, height, title, x, y); }
    Window::~Window() { 
        ::DestroyIcon(m_icon);
        ::DestroyCursor(m_cursor);
        ::ReleaseDC(m_window, m_deviceContext);
        ::UnregisterClassW(TEXT("IWindow::Window"), GetModuleHandle(nullptr));
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
        m_icon = LoadIcon(nullptr, IDI_APPLICATION);
        m_cursor = LoadCursor(nullptr, IDC_ARROW);

        HINSTANCE instance = GetModuleHandle(nullptr);

        WNDCLASS wc{};
        wc.lpfnWndProc = s_WindowCallback;
        wc.hInstance = instance;
        wc.lpszClassName = TEXT("IWindow::Window");
        wc.style = CS_OWNDC | CS_DBLCLKS ;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
        wc.hCursor = m_cursor;
        wc.hIcon = m_icon;
        

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
        Window* iWindow = (Window*)GetWindowLongPtr(window, GWLP_USERDATA);

        if (!iWindow) 
            return ::DefWindowProc(window, msg, wparam, lparam);
        

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
            m_x = GET_X_LPARAM(lparam);
            m_y = GET_Y_LPARAM(lparam);
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

        case WM_SETCURSOR: {
            ::SetCursor(m_cursor);
            return true; // we have to return true
            break;
        }
        default:
            break;
        }

        return ::DefWindowProc(window, msg, wparam, lparam);
    }

    IVector2 Window::GetWindowSize() {
        RECT rect;
        ::GetClientRect(m_window, &rect);

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

    bool Window::IsKeyDown(Key key) { return m_keys[(int)key]; }

    bool Window::IsKeyUp(Key key) { return !IsKeyDown(key); }

    bool Window::IsMouseButtonDown(MouseButton button) { return m_mouseButtons[(int)button]; }
    bool Window::IsMouseButtonDoubleClicked(MouseButton button) { return m_mouseButtonsDbl[(int)button]; }
    bool Window::IsMouseButtonUp(MouseButton button) { return !IsMouseButtonDown(button) || IsMouseButtonDoubleClicked(button); }


    IVector2 Window::GetWindowPosition() {
        WINDOWPLACEMENT windowPlacement{};
        
        ::GetWindowPlacement(m_window, &windowPlacement);

        return IVector2{(int64_t)windowPlacement.rcNormalPosition.left, (int64_t)windowPlacement.rcNormalPosition.top};
    }

    void Window::SetUserPointer(void* ptr) { m_userPtr = ptr; }

    void* Window::GetUserPointer() { return m_userPtr; }

    void Window::SetPosCallback(WindowPosCallback callback) { m_posCallback = callback; }
    void Window::SetSizeCallback(WindowSizeCallback callback) { m_sizeCallback = callback; }
    void Window::SetKeyCallback(KeyCallback callback) { m_keyCallback = callback; }
    void Window::SetMouseMoveCallback(MouseMoveCallback callback) { m_mouseMovecallback = callback; }
    void Window::SetMouseButtonCallback(MouseButtonCallback callback) { m_mouseButtonCallback = callback; }


    IVector2 Window::GetMousePosition() { return IVector2{ m_mouseX, m_mouseY }; }

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
            (int)monitor.size.x,
            (int)monitor.size.y,
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }

    bool Window::IsFullscreen() { return m_fullscreen; }

    // Orignally was glfw3's implementation reworked to work with IWindow
    HANDLE CreateImage(Image image, uint32_t hotX, uint32_t hotY, bool isIcon) {
        HANDLE imageHandle;
        HBITMAP color, mask;
        BITMAPV5HEADER bi;
        ICONINFO ii;
        unsigned char* target = NULL;
        unsigned char* source = image.data;

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

        color = ::CreateDIBSection(deviceContext,
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
        ii.hbmColor = color;

        imageHandle = ::CreateIconIndirect(&ii);

        ::DeleteObject(color);
        ::DeleteObject(mask);

        return imageHandle;
    }

    // Implementation came from GLFW
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

    NativeDeviceContext& Window::GetNativeDeviceContext() { return m_deviceContext; }
}