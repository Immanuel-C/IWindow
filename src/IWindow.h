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
#pragma once

#include <vector>
#include <array>
#include <string>

#include "IWindowPlatform.h"
#include "IWindowCodes.h"

#include <functional>

namespace IWindow {
    struct IVector2 {
        int64_t x, y;
    };

    struct Vector2 {
        float x, y;
    };

    struct Image {
        int32_t width, height;
        uint8_t* data;
    };

    struct Monitor {
        IVector2 size, position;
        std::wstring name;
    };


    class Window {
    private:
        // Input callbacks
        typedef std::function<void(Window&, Key, InputState)> KeyCallback;
        typedef std::function<void(Window&, int64_t, int64_t)> MouseMoveCallback;
        typedef std::function<void(Window&, float, float)> MouseScrollCallback;
        typedef std::function<void(Window&, MouseButton, InputState)> MouseButtonCallback;

        typedef MouseMoveCallback WindowPosCallback;
        typedef WindowPosCallback WindowSizeCallback;
    public:
        Window() = default;
        Window(int64_t width, int64_t height, const std::string& title, int64_t x = 100, int64_t y = 100);
        ~Window(); 

        bool Create(int64_t width, int64_t height, const std::string& title, int64_t x = 100, int64_t y = 100);

        void Update();

        bool IsRunning();

        NativeWindowHandle& GetNativeWindowHandle(); 

        IVector2 GetWindowSize();
        IVector2 GetWindowPosition();

        IVector2 GetMousePosition();

        void SetWindowSize(int64_t width, int64_t height);
        void SetWindowPosition(int64_t x, int64_t y);

        bool IsKeyDown(Key key);
        template<typename... Args>
        bool IsKeyDown(Key key, Args... args) { return IsKeyDown(key) && IsKeyDown(args...); }

        bool IsKeyUp(Key key);
        template<typename... Args>
        bool IsKeyUp(Key key, Args... args) { return IsKeyUp(key) && IsKeyUp(args...); }

        bool IsMouseButtonDown(MouseButton button);
        template<typename... Args>
        bool IsMouseButtonDown(MouseButton button, Args... args) { return IsMouseButtonDown(button) && IsMouseButtonDown(args...); }

        bool IsMouseButtonUp(MouseButton button);
        template<typename... Args>
        bool IsMouseButtonUp(MouseButton button, Args... args) { return IsMouseButtonUp(button) && IsMouseButtonUp(args...); }

        Vector2 GetMouseScrollOffset();

        void SetUserPointer(void* ptr);
        void* GetUserPointer();

        void SetPosCallback(WindowPosCallback callback);
        void SetSizeCallback(WindowSizeCallback callback);
        void SetKeyCallback(KeyCallback callback);
        void SetMouseMoveCallback(MouseMoveCallback callback);
        void SetMouseButtonCallback(MouseButtonCallback callback);
        void SetMouseScrollCallback(MouseScrollCallback callback);

        Monitor GetPrimaryMonitor();
        std::vector<Monitor> GetAllMonitors();

        void Center(Monitor monitor);
        void Fullscreen(bool fullscreen, Monitor monitor);
        bool IsFullscreen();

        void SetIcon(Image image);
        void SetCursor(Image image, uint32_t hotX, uint32_t hotY);
        // Win32 (Windows) Only
        void SetIcon(NativeIconID iconID);
        // Win32 (Windows) Only
        void SetCursor(NativeCursorID cursorID);

        NativeDeviceContext& GetNativeDeviceContext();

        // X11 (Linux) only
        // On Windows this will return nullptr
        X11Display GetX11Display();
        
        void operator=(Window&) = delete;
        void operator=(Window&&) = delete;
        Window(Window&) = delete;
        Window(Window&&) = delete;
    private:
#if defined (_WIN32)
        LRESULT CALLBACK WindowCallback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);
        static LRESULT CALLBACK s_WindowCallback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);
#endif
        int64_t m_width = 0, m_height = 0, m_oldWidth = 0, m_oldHeight = 0, m_x = 0, m_y = 0;
        int64_t m_mouseX = 0, m_mouseY = 0;
        float m_scrollOffsetX = 0.0f, m_scrollOffsetY = 0.0f;
        std::string m_title = "";

        bool m_fullscreen = false;

        bool m_running = true;

        NativeWindowHandle m_window;
        X11Display m_display;

        std::array<bool, (int64_t)Key::Max> m_keys{ false };
        std::array<bool, (int64_t)MouseButton::Max> m_mouseButtons{ false };

        static void DefaultWindowPosCallback(Window&, int64_t, int64_t) {}
        static void DefaultWindowSizeCallback(Window&, int64_t, int64_t) {}
        static void DefaultKeyCallback(Window&, Key, InputState) {}
        static void DefaultMouseMoveCallback(Window&, int64_t, int64_t) {}
        static void DefaultMouseButtonCallback(Window&, MouseButton, InputState) {}
        static void DefaultMouseScrollCallback(Window&, float, float) {}

        WindowPosCallback m_posCallback = DefaultWindowPosCallback;
        WindowSizeCallback m_sizeCallback = DefaultWindowSizeCallback;
        KeyCallback m_keyCallback = DefaultKeyCallback;
        MouseMoveCallback m_mouseMovecallback = DefaultMouseMoveCallback;
        MouseButtonCallback m_mouseButtonCallback = DefaultMouseButtonCallback;
        MouseScrollCallback m_mouseScrollCallback = DefaultMouseScrollCallback;

        NativeDeviceContext m_deviceContext;

        NativeCursor m_cursor;
        NativeIcon m_icon;

        void* m_userPtr;

    };
}
