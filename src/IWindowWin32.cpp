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

#include "IWindowWindow.h"

#include <Shellapi.h>
#include <iostream>
#include <locale>
#include <codecvt>

namespace IWindow {
    uint32_t Window::m_sWindowCount;

    Window::Window(const Vector2<int32_t>& size, const std::u16string& title, const Monitor& monitor, const Vector2<int32_t>& position, const Style& style) { Create(size, title, monitor, position, style); }

    bool Window::Create(const Vector2<int32_t>& size, const std::u16string& title, const Monitor& monitor, const Vector2<int32_t>& position, const Style& style) {
        // User did not call IWindow::Initialize
        if (GetVersion() == "") {
            ::MessageBoxA(nullptr, "You have to initialize IWindow before creating a window!", "Error", MB_ICONEXCLAMATION | MB_OK);
            return false;
        }

        m_size = size;
        m_oldSize = size;
        m_position = position;
        m_title = title;
        m_userPtr = nullptr;
        m_running = true;
        m_fullscreen = false;
        m_focused = false;
        m_mouseEntered = false;
        m_iconified = false;
        m_windowStyle = 0;

        m_keys.resize((size_t)Key::Max);
        m_keysPressedOnce.resize((size_t)Key::Max);
        m_mouseButtons.resize((size_t)MouseButton::Max);

        m_cursor = ::LoadCursor(nullptr, IDC_ARROW);
        m_icon = ::LoadIcon(nullptr, IDI_APPLICATION);

        HINSTANCE instance = GetModuleHandle(nullptr);

        // for multi-window support
        m_sWindowCount++;

        m_windowIndex = m_sWindowCount;

        std::wstring className = L"IWindow::Window" + m_windowIndex;

        WNDCLASS wc{};
        wc.lpfnWndProc = s_WindowCallback;
        wc.hInstance = instance;
        wc.lpszClassName = className.c_str();
        wc.style = CS_OWNDC;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
        wc.hCursor = m_cursor;
        wc.hIcon = m_icon;
        
        // If already registered dont register again
        if (!::RegisterClass(&wc) ) {
            ::MessageBoxA(nullptr, "Failed to register window class!", "Error", MB_ICONEXCLAMATION | MB_OK);
            return false;
        }
        
        m_window = 
        ::CreateWindowEx
        (
            WS_EX_APPWINDOW,                                            // Window Type?
            className.c_str(),                                          // Class Name
            (LPWSTR)m_title.c_str(),                                    // Window Name
            m_windowStyle,                                              // Styles
            (int)position.x,                                            // PosX
            (int)position.y,                                            // PosY
            (int)size.width,                                            // SizeX
            (int)size.height,                                           // SizeY
            nullptr,                                                    // Parent Window
            nullptr,                                                    // HMENU?
            instance,                                                   // hinstance
            nullptr                                                     // lpParam LPVOID?
        );

        if (!m_window) {
            ::MessageBoxA(nullptr, "Failed to create window!", "Error", MB_ICONEXCLAMATION | MB_OK);
            return false; 
        }

        ::DragAcceptFiles(m_window, true);

        SetStyle(style);

        // Put window to middle of the screen if the user didn't provide a position.
        if (position.IsEmpty()) { 
            Center(monitor); 
        }

        ::SetWindowLongPtr(m_window, GWLP_USERDATA, (LONG_PTR)this);

        ::ShowWindow(m_window, SW_SHOW);

        m_deviceContext = GetDC(m_window);

        m_timeMS = std::chrono::high_resolution_clock::now();

        return true;
    }

    void Window::Destroy() {
        ::DestroyIcon(m_icon);
        ::DestroyCursor(m_cursor);
        ::ReleaseDC(m_window, m_deviceContext);
        ::UnregisterClassW(L"IWindow::Window" + m_windowIndex, GetModuleHandle(nullptr));
        ::DestroyWindow(m_window);
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

    void Window::WaitForEvent() {
        ::WaitMessage();
        Update();
    }

    bool Window::IsRunning() const { return m_running; }

    LRESULT CALLBACK Window::s_WindowCallback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam) {
        Window* iWindow = (Window*)GetWindowLongPtr(window, GWLP_USERDATA);

        if (!iWindow) 
            return ::DefWindowProc(window, msg, wparam, lparam);
        

        return iWindow->WindowCallback(window, msg, wparam, lparam);
    }

    bool Window::operator==(IWindow::Window& window) { return m_window == window.GetNativeWindowHandle(); }
    bool Window::operator!=(IWindow::Window& window) { return m_window != window.GetNativeWindowHandle(); }

    static KeyModifier GetKeyModifiers() {
        KeyModifier mods = KeyModifier::None;

        if (GetKeyState(VK_SHIFT) & 0x8000)
            mods |= KeyModifier::Shift;
        if (GetKeyState(VK_CONTROL) & 0x8000)
            mods |= KeyModifier::Control;
        if (GetKeyState(VK_MENU) & 0x8000)
            mods |= KeyModifier::Alt;
        if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000)
            mods |= KeyModifier::Super;
        if (GetKeyState(VK_CAPITAL) & 1)
            mods |= KeyModifier::CapsLock;
        if (GetKeyState(VK_NUMLOCK) & 1)
            mods |= KeyModifier::NumLock;

        return mods;
    }

    LRESULT CALLBACK Window::WindowCallback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam) {
        switch (msg)
        {
        case WM_CLOSE:
            m_running = false;        

            return 0;
        case WM_MOVE: {
            // if lparam is less than zero than it will act like a uint
            m_position.x = GET_X_LPARAM(lparam);
            m_position.y = GET_Y_LPARAM(lparam);
            m_posCallback(*this, m_position);

            return 0;
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
            m_mousePosition.x = GET_X_LPARAM(lparam);
            m_mousePosition.y = GET_Y_LPARAM(lparam);
            m_mouseMovecallback(*this, m_mousePosition);

            return 0;
        }
        case WM_MOUSELEAVE: {
            m_mouseEntered = false;
            m_mouseEnteredCallback(*this, m_mouseEntered);

            return 0;
        }

        case WM_DPICHANGED: {
            RECT rect{};
            ::SetRect(&rect, 0, 0, LOWORD(lparam), HIWORD(lparam));
            ::AdjustWindowRectExForDpi(&rect, m_windowStyle, false, WS_EX_APPWINDOW, ::GetDpiForWindow(m_window));

            m_framebufferSize = { (int32_t)(rect.right - -(rect.left)), (int32_t)(rect.bottom - -(rect.top)) };

            m_framebufferSizeCallback(*this, m_framebufferSize);

            return 0;
        }

        case WM_SIZE: {
            bool iconified = wparam == SIZE_MINIMIZED ? true : false;
            bool maximized = wparam == SIZE_MAXIMIZED ? true : false;

            if (iconified != iconified) {
                m_iconified = iconified;
                m_inconifiedCallback(*this, false);
            }
            if (maximized != maximized) {
                m_maximized = maximized;
                m_maximizedCallback(*this, false);
            }

            m_size = GetWindowSize();
            m_sizeCallback(*this, m_size);

            RECT rect{};
            ::SetRect(&rect, 0, 0, LOWORD(lparam), HIWORD(lparam));
            ::AdjustWindowRectExForDpi(&rect, m_windowStyle, false, WS_EX_APPWINDOW, ::GetDpiForWindow(m_window));

            m_framebufferSize = { (int32_t)(rect.right - -(rect.left)), (int32_t)(rect.bottom - -(rect.top)) };

            m_framebufferSizeCallback(*this, m_framebufferSize);
            return 0;        
        }

        case WM_SYSKEYDOWN: 
        case WM_SYSKEYUP: 
        case WM_KEYUP: 
        case WM_KEYDOWN: {
            const InputState inputState = (HIWORD(lparam) & KF_UP) ? InputState::Up : InputState::Down;
            m_mods = GetKeyModifiers();
            int32_t repeat = (HIWORD(lparam) & KF_REPEAT);
            uint32_t scanCode = ((lparam & 0x00ff0000) >> 16);
            // KF_EXTENDED broken wtf Win32!?
            bool extented = (lparam & 0x01000000) != 0;
            
            Key key = Win32KeyCodeToIWindowKey(wparam);
            
            if (key == Key::Max) {
                if (!extented && wparam == VK_MENU) key = Key::LAlt;
                if (extented && wparam == VK_MENU) key = Key::RAlt;
                if (!extented && wparam == VK_CONTROL) key = Key::LControl;
                if (extented && wparam == VK_CONTROL) key = Key::RControl;
                if (wparam == VK_SHIFT) {
                    WPARAM mappedCode = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
                    key = mappedCode == VK_LSHIFT ? Key::LShift : Key::RShift;
                }
            }

            // Unsupported Key
            if (key == Key::Max) break;

            if (inputState == InputState::Down) {
                m_keys[(uint64_t)key] = true;
                m_keysPressedOnce[(uint64_t)key] = repeat > 0 ? false : true;
            }
            else {
                m_keys[(uint64_t)key] = false;
                m_keysPressedOnce[(uint64_t)key] = false;
            }

            // Auto iconify if super key is pressed and window is fullscreen.
            if ((key == Key::LSuper || key == Key::RSuper) && (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN) && m_fullscreen)
                ::ShowWindow(m_window, SW_MINIMIZE);

            m_keyCallback(*this, key, m_mods, inputState, repeat > 0);

            return 0;
        }

        case WM_SYSCOMMAND: {
            // Checks if the menu was selected via alt / alt + something. We want control over alt though so we have to return 0 in that case.
            if (wparam == SC_KEYMENU && HIWORD(lparam) <= 0) return 0;
            break;
        }

        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP: {
            m_mods = GetKeyModifiers();

            InputState inputState;
            MouseButton button;

            if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP) button = MouseButton::Left;
            else if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP) button = MouseButton::Right;
            else if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP) button = MouseButton::Middle;
            else if (GET_XBUTTON_WPARAM(wparam) == XBUTTON1) button = MouseButton::Side1;
            else button = MouseButton::Side2;

            if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_XBUTTONDOWN)
                inputState = InputState::Down;
            else
                inputState = InputState::Up;

            m_mouseButtons[(int)button] = true;
            m_mouseButtonCallback(*this, button, m_mods, inputState);

            if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONUP)
                return 1;

            return 0;
        }
        case WM_SETCURSOR: {
            if (LOWORD(lparam) == HTCLIENT) {
                ::SetCursor(m_cursor);
                return 1;
            }

            break;
        }
        // Y axis
        case WM_MOUSEWHEEL: {
            m_scrollOffset.y = (int16_t)HIWORD(wparam) / (float)WHEEL_DELTA;
            m_scrollOffset.x = 0.0f;
            m_mouseScrollCallback(*this, m_scrollOffset);
            return 0;
        }
        // X axis
        case WM_MOUSEHWHEEL: {
            // X axis inverted for consistency with x11
            m_scrollOffset.x = -((int16_t)HIWORD(wparam) / (float)WHEEL_DELTA);
            m_scrollOffset.y = 0.0f;
            m_mouseScrollCallback(*this, m_scrollOffset);
            return 0;
        }
        // User focused on the window
        case WM_SETFOCUS: {
            m_focused = true;
            m_windowFocusCallback(*this, m_focused);
            return 0;
        }

        // User is not focused on the window
        case WM_KILLFOCUS: {
            m_focused = false;
            m_windowFocusCallback(*this, m_focused);
            return 0;
        }

        case WM_UNICHAR: 
        case WM_SYSCHAR: 
        case WM_CHAR: {
            if (wparam == UNICODE_NOCHAR && msg == WM_UNICHAR) {
                // Not sent by windows but by a third party.
                return 1;
            }

            // Dont input char if its backspace, ...
            if (wparam < 32 || (wparam > 126 && wparam < 160))
                return 0;

            m_charCallback(*this, (char32_t)wparam, GetKeyModifiers());

            return 0;
        }
        case WM_DROPFILES: {
            HDROP drop = (HDROP)wparam;
            POINT mousePosition{};

            ::DragQueryPoint(drop, &mousePosition);

            const uint32_t amountOfFiles = ::DragQueryFile(drop, 0xFFFFFFFF, nullptr, 0);
            std::vector<std::wstring> paths{};
            paths.resize(amountOfFiles);

            for (uint32_t i = 0; i < amountOfFiles; i++) {
                const uint32_t length = ::DragQueryFile(drop, i, nullptr, 0);
                paths[i].resize((size_t)(length + 1));

                ::DragQueryFile(drop, i, paths[i].data(), (size_t)(length + 1));
            }

            m_pathDropCallback(*this, paths, { (int32_t)mousePosition.x, (int32_t)mousePosition.y });

            ::DragFinish(drop);
            return 0;
        }

        default:
            break;
        }

        return ::DefWindowProc(window, msg, wparam, lparam);
    }

    Vector2<int32_t> Window::GetWindowSize() const {
        RECT rect;
        ::GetClientRect(m_window, &rect);

        return { (int32_t)rect.right - rect.left, (int32_t)rect.bottom - rect.top };
    }

    void Window::SetWindowSize(const Vector2<int32_t>& size) {
        m_size = size;

        ::SetWindowPos(m_window, nullptr, (int)m_position.x, (int)m_position.y, (int)m_size.width, (int)m_size.height, SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
    }

    void Window::SetWindowPosition(const Vector2<int32_t>& position) {
        m_position = position;

        ::SetWindowPos(m_window, nullptr, (int)m_position.x, (int)m_position.y, (int)m_size.width, (int)m_size.height, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
    }

    void Window::SetMousePosition(const Vector2<int32_t>& position) { 
        POINT pt{};
        pt.x = position.x;
        pt.y = position.y;
        
        if (!::ClientToScreen(m_window, &pt)) {
            std::cout << "Failed to set mouse position!\n";
            return;
        }

        m_mousePosition.x = pt.x;
        m_mousePosition.y = pt.y;

        ::SetCursorPos(pt.x, pt.y); 
    }

    Vector2<int32_t> Window::GetMousePosition() const { return m_mousePosition; }
    Vector2<float> Window::GetMouseScrollOffset() const { return m_scrollOffset; }
    Vector2<int32_t> Window::GetWindowPosition() const { return m_position; }
    Vector2<int32_t> Window::GetFramebufferSize() const { return m_framebufferSize; }

    bool Window::IsKeyDown(Key key, KeyModifier mods) { return m_keys[(int64_t)key] && IsKeyModifiersDown(mods); }
    bool Window::IsKeyUp(Key key, KeyModifier mods) { return !IsKeyDown(key) && IsKeyModifiersUp(mods); }

    bool Window::IsKeyJustPressed(Key key, KeyModifier mods) { return m_keysPressedOnce[(int64_t)key] && IsKeyModifiersDown(mods); }

    bool Window::IsMouseButtonDown(MouseButton button, KeyModifier mods) {  return m_mouseButtons[(int)button] && IsKeyModifiersDown(mods); }
    bool Window::IsMouseButtonUp(MouseButton button, KeyModifier mods) { return !IsMouseButtonDown(button) && IsKeyModifiersUp(mods); }

    bool Window::IsKeyModifiersDown(KeyModifier mods)
    {
        if ((uint64_t)(mods & KeyModifier::Alt) && !(uint64_t)(m_mods & KeyModifier::Alt)) return false;
        if ((uint64_t)(mods & KeyModifier::CapsLock) && !(uint64_t)(m_mods & KeyModifier::CapsLock)) return false;
        if ((uint64_t)(mods & KeyModifier::Control) && !(uint64_t)(m_mods & KeyModifier::Control)) return false;
        if ((uint64_t)(mods & KeyModifier::Shift) && !(uint64_t)(m_mods & KeyModifier::Shift)) return false;
        if ((uint64_t)(mods & KeyModifier::NumLock) && !(uint64_t)(m_mods & KeyModifier::NumLock)) return false;
        if ((uint64_t)(mods & KeyModifier::Super) && !(uint64_t)(m_mods & KeyModifier::Super)) return false;

        return true;
    }

    bool Window::IsKeyModifiersUp(KeyModifier mods)
    {
        if ((uint64_t)(mods & KeyModifier::Alt) && (uint64_t)(m_mods & KeyModifier::Alt)) return false;
        if ((uint64_t)(mods & KeyModifier::CapsLock) && (uint64_t)(m_mods & KeyModifier::CapsLock)) return false;
        if ((uint64_t)(mods & KeyModifier::Control) && (uint64_t)(m_mods & KeyModifier::Control)) return false;
        if ((uint64_t)(mods & KeyModifier::Shift) && (uint64_t)(m_mods & KeyModifier::Shift)) return false;
        if ((uint64_t)(mods & KeyModifier::NumLock) && (uint64_t)(m_mods & KeyModifier::NumLock)) return false;
        if ((uint64_t)(mods & KeyModifier::Super) && (uint64_t)(m_mods & KeyModifier::Super)) return false;

        return true;
    }

    void Window::SetUserPointer(void* ptr) { m_userPtr = ptr; }
    void* Window::GetUserPointer() const { return m_userPtr; }

    void Window::Center(Monitor monitor) {
        // If the monitor is not a primary monitor we have to offset the position.
        Vector2<int32_t> offset = monitor.position != Monitor::GetPrimaryMonitor().position ? monitor.position : Vector2<int32_t>{ 0, 0 };
        SetWindowPosition({ ((monitor.size.x - m_size.width) / 2) + offset.x , ((monitor.size.y - m_size.height) / 2) + offset.y });
    }

    void Window::Fullscreen(bool fullscreen, Monitor monitor) {
        if (m_fullscreen == fullscreen)
            return;

        m_fullscreen = fullscreen;

        // No fullscreen
        if (!m_fullscreen) {
            ::SetWindowLongPtr(m_window, GWL_EXSTYLE, WS_EX_LEFT);
            ::SetWindowLongPtr(m_window, GWL_STYLE, m_windowStyle);
            SetWindowSize(m_oldSize);
            Center(monitor);
            return;
        }

        // Fullscreen
        m_oldSize = m_size;

        ::SetWindowLongPtr(m_window, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
        ::SetWindowLongPtr(m_window, GWL_STYLE, WS_POPUP | WS_VISIBLE);

        ::SetWindowPos(m_window, nullptr,
            (int)monitor.position.x, (int)monitor.position.y,
            (int)monitor.size.x, (int)monitor.size.y,
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }

    bool Window::IsFullscreen() const { return m_fullscreen; }

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
        bi.bV5Width = (int32_t)image.size.width;
        // Doing image.height will make the image upside down
        bi.bV5Height = -(int32_t)(image.size.height);
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


        mask = ::CreateBitmap((int32_t)image.size.width, (int32_t)image.size.height, 1, 1, NULL);

        ::ReleaseDC(nullptr, deviceContext);

        for (uint32_t i = 0; i < image.size.width * image.size.height; i++) {
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
        ::DestroyIcon(m_icon);

        m_icon = (NativeIcon)CreateImage(image, 0, 0, true);

        ::SendMessage(m_window, WM_SETICON, ICON_SMALL, (LPARAM)m_icon);
        ::SendMessage(m_window, WM_SETICON, ICON_BIG, (LPARAM)m_icon);
    }

    void Window::SetCursor(Image image, Vector2<int32_t> hot) {
        ::DestroyCursor(m_cursor);
        m_cursor = (NativeCursor)CreateImage(image, hot.x, hot.y, false);
    }

    void Window::SetIcon(IconID iconID) {
        ::DestroyIcon(m_icon);

        m_icon = ::LoadIcon(nullptr, IDI_APPLICATION);

        ::SendMessage(m_window, WM_SETICON, ICON_SMALL, (LPARAM)m_icon);
        ::SendMessage(m_window, WM_SETICON, ICON_BIG, (LPARAM)m_icon);
    }

    void Window::SetCursor(CursorID cursorID) {
        ::DestroyCursor(m_cursor);

        if (cursorID != CursorID::Hidden)
            m_cursor = ::LoadCursor(nullptr, CursorIDToWin32ID(cursorID));
        else
            m_cursor = nullptr;    
    }

    std::string Window::GetClipboardText() const {
        if (!::OpenClipboard(nullptr)) return std::string{};

        // CF_TEXT = ASCII Text
        HANDLE data = ::GetClipboardData(CF_TEXT);
        if (data == nullptr) return std::string{};

        // Lock the data so we cant ctrl-c a new string while copying the current string
        const char* cData  = (char*)::GlobalLock(data);
        if (!cData) return std::string{};

        ::GlobalUnlock(data);

        ::CloseClipboard();

        std::string clipboardText{ cData };

        ::GlobalFree(data);

        return clipboardText;
    }

    void Window::SetClipboardText(const std::string& text)
    {
        HGLOBAL mem = ::GlobalAlloc(GMEM_MOVEABLE, (text.size() + 1) * sizeof(char));

        if (!mem) {
            std::cout << "Cant set clipboard text!\n";
            return;
        }

        memcpy(::GlobalLock(mem), text.c_str(), (text.size() + 1) * sizeof(char));
        ::GlobalUnlock(mem);

        if (!::OpenClipboard(nullptr)) {
            auto lasterror = GetLastError();
            std::cout << "Cant set clipboard text!\n";
            return;
        }

        ::EmptyClipboard();
        ::SetClipboardData(CF_TEXT, mem);

        ::GlobalFree(mem);

        ::CloseClipboard();
    }

    double Window::GetTime() const { 
        std::chrono::duration<double, std::milli> dur = std::chrono::high_resolution_clock::now() - m_timeMS;
        return dur.count(); 
    }

    bool Window::IsFocused() const { return m_focused; }
    bool Window::IsIconified() const { return m_iconified; }
    bool Window::IsMaximized() const { return m_maximized; }

    void IWindow::Window::SetTitle(const std::u16string& title) {  
        m_title = title;
        ::SetWindowText(m_window, (LPCWSTR)title.c_str());
    }


    void Window::SetStyle(Style style) {
        // All styles ignored when in fullscreen.
        if (m_fullscreen) return;

        m_windowStyle = GetWindowStyle(m_window);

        if ((NativeStyle)(style & Style::Default)) {
            m_windowStyle = 0;
            m_windowStyle |= WS_OVERLAPPEDWINDOW;
        }

        if ((NativeStyle)(style & Style::Resizable)) {
            m_windowStyle |= WS_SIZEBOX;
            m_windowStyle |= WS_MAXIMIZEBOX;
        }
        if ((NativeStyle)(style & Style::NotResizable)) {
            m_windowStyle &= ~WS_SIZEBOX; 
            m_windowStyle &= ~WS_MAXIMIZEBOX;
        }

        if ((NativeStyle)(style & Style::Visible)) {
            ShowWindow(m_window, SW_SHOW);
        }
        if ((NativeStyle)(style & Style::NotVisible)) {
            ShowWindow(m_window, SW_HIDE);
        }

        if ((NativeStyle)(style & Style::Decorated)) {
            m_windowStyle |= WS_OVERLAPPEDWINDOW;
        }
        if ((NativeStyle)(style & Style::NotDecorated)) {
            m_windowStyle &= ~(WS_OVERLAPPEDWINDOW);
        }

        if ((NativeStyle)(style & Style::Maximize)) {
            ShowWindow(m_window, SW_MAXIMIZE);
        }
        if ((NativeStyle)(style & Style::Restore)) {
            ShowWindow(m_window, SW_RESTORE);
        }

        ::SetWindowLong(m_window, GWL_STYLE, m_windowStyle);
    }

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

    Window::WindowIconifiedCallback Window::SetWindowIconifiedCallback(WindowIconifiedCallback callback) {
        WindowIconifiedCallback oldcallback = m_inconifiedCallback;
        m_inconifiedCallback = callback;
        return oldcallback;
    }

    Window::WindowMaximizedCallback Window::SetWindowMaximizedCallback(WindowMaximizedCallback callback) {
        WindowMaximizedCallback oldcallback = m_maximizedCallback;
        m_maximizedCallback = callback;
        return oldcallback;
    }

    Window::PathDropCallback Window::SetPathDropCallback(PathDropCallback callback)
    {
        PathDropCallback oldCallback = m_pathDropCallback;
        m_pathDropCallback = callback;
        return oldCallback;
    }

    const NativeWindowHandle& Window::GetNativeWindowHandle() const { return m_window; };
    const NativeDeviceContext& Window::GetNativeDeviceContext() const { return m_deviceContext; }
}
#endif