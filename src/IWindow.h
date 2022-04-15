#pragma once

#include <vector>
#include <array>
#include <string>


#if defined (_WIN32)
#define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

#include "IWindowPlatform.h"
#include "IWindowCodes.h"

#include <functional>

namespace IWindow {
    struct WindowSize {
        int64_t width, height;
    };
    struct WindowPos {
        int64_t x, y;
    };



    class Window {
    private:
        typedef std::function<void(Window&, int64_t, int64_t)> WindowPosCallback;
        typedef WindowPosCallback WindowSizeCallback;
    public:
            Window() {}
            Window(int64_t width, int64_t height, const std::string& title, int64_t x = 100, int64_t y = 100);

            bool Create(int64_t width, int64_t height, const std::string& title, int64_t x = 100, int64_t y = 100);

            void Update();

            bool IsRunning();

            NativeWindowHandle& GetNativeWindowHandle(); 

            WindowSize GetWindowSize();
            WindowPos GetWindowPosition();

            void SetWindowSize(int64_t width, int64_t height);
            void SetWindowPosiiton(int64_t x, int64_t y);

            bool IsKeyDown(Key key);
            template<typename... Args>
            bool IsKeyDown(Key key, Args... args) { return IsKeyDown(key) && IsKeyDown(args...); }

            bool IsKeyUp(Key key);
            template<typename... Args>
            bool IsKeyUp(Key key, Args... args) { return IsKeyUp(key) && IsKeyUp(args...); }

            ~Window(); 

            void SetPosCallback(WindowPosCallback callback);
            void SetSizeCallback(WindowSizeCallback callback);
    private:
        LRESULT CALLBACK WindowCallback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);
        static LRESULT CALLBACK s_WindowCallback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);

        int64_t m_width = 0, m_height = 0, m_x = 0, m_y = 0;
        std::string m_title = "";

        NativeWindowHandle m_window;

        bool m_running = true;

        static void DefaultWindowPosCallback(Window& window, int64_t x, int64_t y) {}
        static void DefaultWindowSizeCallback(Window& window, int64_t width, int64_t height) {}

        std::array<bool, (int)Key::Max> m_keys{false};


        WindowPosCallback m_posCallback;
        WindowSizeCallback m_sizeCallback;

    };
}