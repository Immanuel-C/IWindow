#pragma once

#include <vector>
#include <array>
#include <string>


#if defined (_WIN32)
#define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <windowsx.h>
#endif

#include "IWindowPlatform.h"
#include "IWindowCodes.h"

#include <functional>

namespace IWindow {
    struct IVector2 {
        int64_t x, y;
    };

    struct Image {
        int32_t width, height;
        uint8_t* data;
    };


    class Window {
    private:
        // Input callbacks
        typedef std::function<void(Window&, Key, InputState)> KeyCallback;
        typedef std::function<void(Window&, int64_t, int64_t)> MouseMoveCallback;
        typedef std::function<void(Window&, MouseButton, InputState, ClickState)> MouseButtonCallback;

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

        bool IsMouseButtonDoubleClicked(MouseButton button);
        template<typename... Args>
        bool IsMouseButtonDoubleClicked(MouseButton button, Args... args) { return IsMouseButtonDoubleClicked(button) && IsMouseButtonDoubleClicked(args...); }

        bool IsMouseButtonUp(MouseButton button);
        template<typename... Args>
        bool IsMouseButtonUp(MouseButton button, Args... args) { return IsMouseButtonUp(button) && IsMouseButtonUp(args...); }

        void SetUserPointer(void* ptr);
        void* GetUserPointer();

        void SetPosCallback(WindowPosCallback callback);
        void SetSizeCallback(WindowSizeCallback callback);
        void SetKeyCallback(KeyCallback callback);
        void SetMouseMoveCallback(MouseMoveCallback callback);
        void SetMouseButtonCallback(MouseButtonCallback callback);

        void Center();
        void Fullscreen(bool fullscreen);
        bool IsFullscreen();

        void SetIcon(Image image);
        void SetCursor(Image image, uint32_t hotX, uint32_t hotY);
        void SetIcon(NativeIconID iconID);
        void SetCursor(NativeCursorID cursorID);

        NativeGLDeviceContext& GetNativeGLDeviceContext();
        
        void operator=(Window&) = delete;
        Window(Window&) = delete;
    private:
        LRESULT CALLBACK WindowCallback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);
        static LRESULT CALLBACK s_WindowCallback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);

        int64_t m_width = 0, m_height = 0, m_oldWidth = 0, m_oldHeight = 0, m_x = 0, m_y = 0;
        int64_t m_mouseX = 0, m_mouseY = 0;
        std::string m_title = "";

        bool m_fullscreen = false;

        bool m_running = true;

        NativeWindowHandle m_window;

        std::array<bool, (int32_t)Key::Max> m_keys{false};
        std::array<bool, (int32_t)MouseButton::Max> m_mouseButtons{false};
        std::array<bool, (int32_t)MouseButton::Max> m_mouseButtonsDbl{false};

        static void DefaultWindowPosCallback(Window&, int64_t, int64_t) {}
        static void DefaultWindowSizeCallback(Window&, int64_t, int64_t) {}
        static void DefaultKeyCallback(Window&, Key, InputState) {}
        static void DefaultMouseMoveCallback(Window&, int64_t, int64_t) {}
        static void DefaultMouseButtonCallback(Window&, MouseButton, InputState, ClickState) {}

        WindowPosCallback m_posCallback = DefaultWindowPosCallback;
        WindowSizeCallback m_sizeCallback = DefaultWindowSizeCallback;
        KeyCallback m_keyCallback = DefaultKeyCallback;
        MouseMoveCallback m_mouseMovecallback = DefaultMouseMoveCallback;
        MouseButtonCallback m_mouseButtonCallback = DefaultMouseButtonCallback;

        NativeGLDeviceContext m_deviceContext;

        NativeCursor m_cursor;
        NativeIcon m_icon;

        void* m_userPtr = nullptr;
    };
}