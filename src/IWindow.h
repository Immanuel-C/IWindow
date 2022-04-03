#pragma once

#include <vector>
#include <string>


#if defined (_WIN32)
#define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

#include "IWindowPlatform.h"

namespace IWindow {
    struct WindowSize {
        uint32_t width, height;
    };
    struct WindowPos {
        uint32_t x, y;
    };

    class Window;

    typedef void (*WindowPosCallback)(Window&, uint32_t, uint32_t);
    typedef void (*WindowSizeCallback)(Window&, uint32_t, uint32_t);


    class Window {
        public:
            Window() {}
            Window(uint32_t width, uint32_t height, const std::string& title, uint32_t x = 100, uint32_t y = 100);

            bool Create(uint32_t width, uint32_t height, const std::string& title, uint32_t x = 100, uint32_t y = 100);

            void Update();

            bool IsRunning();

            NativeWindowHandle& GetNativeWindowHandle(); 

            WindowSize GetWindowSize();
            WindowPos GetWindowPosition();

            void SetWindowSize(uint32_t width, uint32_t height);
            void SetWindowPosiiton(uint32_t x, uint32_t y);

            ~Window(); 


            void SetPosCallback(WindowPosCallback callback);
            void SetSizeCallback(WindowSizeCallback callback);

    private:
        LRESULT CALLBACK WindowCallback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);
        static LRESULT CALLBACK s_WindowCallback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);

        uint32_t m_width = 0, m_height = 0, m_x = 0, m_y = 0;
        std::string m_title = "";

        NativeWindowHandle m_window;

        bool m_running = true;

        static void DefaultWindowPosCallback(Window& window, uint32_t x, uint32_t y) {}
        static void DefaultWindowSizeCallback(Window& window, uint32_t width, uint32_t height) {}

        WindowPosCallback m_posCallback;
        WindowSizeCallback m_sizeCallback;
    };
}