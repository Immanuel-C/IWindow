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

#include "IWindow.h"
#include "IWindowPlatform.h"
#include "IWindowCodes.h"
#include "IWindowCore.h"
#include "IWindowUtils.h"

#include <chrono>
#include <functional>


#ifdef _WIN32
// Name conflict with IWindow::Window::IsMaximized
#undef IsMaximized
#endif // _WIN32

namespace IWindow {
    /// <summary>
    /// Creates a Window using the underlying windowing api of the operating system.
    /// </summary>
    class IWINDOW_API Window {
    public:
        Window() = default;
        /// <summary>
        /// Creates the window. Prefer to use IWindow::Window::Create. Call IWindow::Initialize before creating the window.
        /// </summary>
        /// <param name="size">Size of the window.</param>
        /// <param name="title">The title of the window. Only applies to decorated windows. Must be in utf-16 format.</param>
        /// <param name="monitor">
        /// The monitor the window will be created on. Does not apply if position is overrided.
        /// Default value of this param is the primary monitor.
        /// </param>
        /// <param name="position">
        /// If position is 0, 0 or not set the window will be created in the center of the monitor provided.
        /// Defualt value of this param is 0, 0.
        /// </param>
        /// <param name="style">
        /// The style of the window. Must be a valid IWindow::Style.
        /// Defualt value of this param is IWindow::Style::Defualt.
        /// </param>
        Window(const Vector2<int32_t>& size, const std::wstring& title, const Monitor& monitor = Monitor::GetPrimaryMonitor(), const Vector2<int32_t>& position = {}, const Style& style = Style::Default);
        /// <summary>
        /// Creates the window. Call IWindow::Initialize before creating the window.
        /// </summary>
        /// <param name="size">Size of the window.</param>
        /// <param name="title">The title of the window. Only applies to decorated windows. Must be in utf-16 format.</param>
        /// <param name="monitor">
        /// The monitor the window will be created on. Does not apply if position is overrided.
        /// Default value of this param is the primary monitor.
        /// </param>
        /// <param name="position">
        /// If position is 0, 0 or not set the window will be created in the center of the monitor provided.
        /// Defualt value of this param is 0, 0.
        /// </param>
        /// <param name="style">
        /// The style of the window. Must be a valid IWindow::Style.
        /// Defualt value of this param is IWindow::Style::Defualt.
        /// </param>
        bool Create(const Vector2<int32_t>& size, const std::wstring& title, const Monitor& monitor = Monitor::GetPrimaryMonitor(), const Vector2<int32_t>& position = {}, const Style& style = Style::Default);
        /// <summary>
        /// Destroys the window.
        /// </summary>
        void Destroy();
        /// <summary>
        /// Updates the window and gets any event such as mouse movement, resizing the window, keyboard input, etc. 
        /// </summary>
        void Update();
        /// <summary>
        /// The current thread will be paused until a event occurs.
        /// </summary>
        void WaitForEvent();
        /// <summary>
        /// Checks if the window is still open. 
        /// </summary>
        /// <returns>
        /// true if the window is still open.
        /// false if the window was closed.
        /// </returns>
        bool IsRunning() const;

        /// <summary>
        /// Gets the underlying windowing api's window handle.
        /// </summary>
        /// <returns>
        /// On Windows this method with return a HWND.
        /// </returns>
        const NativeWindowHandle& GetNativeWindowHandle() const; 
        /// <summary>
        /// Gets the underlying windowing api's device context.
        /// </summary>
        /// <returns>
        /// On Windows this method with return a HDC.
        /// </returns>
        const NativeDeviceContext& GetNativeDeviceContext() const;

        /// <returns>Size of the window including decorations, **not** adjusted for high dpi monitors.</returns>
        Vector2<int32_t> GetWindowSize() const;
        /// <returns>Position of the window in screen space.</returns>
        Vector2<int32_t> GetWindowPosition() const;
        /// <returns>Size of the windows client area, adjusted for high dpi monitors.</returns>
        Vector2<int32_t> GetFramebufferSize() const;
        /// <summary>
        /// Set the window size in screen space.
        /// </summary>
        /// <param name="size">Size in screen space.</param>
        void SetWindowSize(const Vector2<int32_t>& size);
        /// <summary>
        /// Set the window position in screen space.
        /// </summary>
        /// <param name="position">Position in screen space.</param>
        void SetWindowPosition(const Vector2<int32_t>& position);

        /// <returns>Mouse position in client space.</returns>
        Vector2<int32_t> GetMousePosition() const;
        /// <summary>
        /// Sets the mouse position in client space.
        /// </summary>
        /// <param name="position">Position in client space.</param>
        void SetMousePosition(const Vector2<int32_t>& position);

        /// <summary>
        /// Check if a key modifier is down.
        /// </summary>
        /// <param name="mods">Modifiers to check.</param>
        /// <returns>
        /// true if modifier is down.
        /// false if modifier is not down.
        /// </returns>
        bool IsKeyModifiersDown(KeyModifier mods);
        /// <summary>
        /// Check if a key modifier is up.
        /// </summary>
        /// <param name="mods">Modifiers to check.</param>
        /// <returns>
        /// true if modifier is not down.
        /// false if modifier is down.
        /// </returns>
        bool IsKeyModifiersUp(KeyModifier mods);
        /// <summary>
        /// Check if key and modifiers are down.
        /// </summary>
        /// <param name="key">Key to check.</param>
        /// <param name="mods">Modifiers to check.</param>
        /// <returns>
        /// true if key and modifiers are down.
        /// false if key and modifiers are not down.
        /// </returns>
        bool IsKeyDown(Key key, KeyModifier mods = KeyModifier::None);
        /// <summary>
        /// Check if key and modifiers are up.
        /// </summary>
        /// <param name="key">Key to check.</param>
        /// <param name="mods">Modifiers to check.</param>
        /// <returns>
        /// true if key and modifiers are not down.
        /// false if key and modifiers are down.
        /// </returns>
        bool IsKeyUp(Key key, KeyModifier mods = KeyModifier::None);
        /// <summary>
        /// Check if key and modifiers were just pressed in the last update.
        /// </summary>
        /// <param name="key">Key to check.</param>
        /// <param name="mods">Modifiers to check.</param>
        /// <returns>
        /// true if key and modifiers are down and were not pressed for more than 1 update call.
        /// false if key and modifiers are down and were pressed for more than 1 update call or if they are up.
        /// </returns>
        bool IsKeyJustPressed(Key key, KeyModifier mods = KeyModifier::None);
        /// <summary>
        /// Check if mouse button and modifiers are down.
        /// </summary>
        /// <param name="button">Button to check.</param>
        /// <param name="mods">Modifiers to check.</param>
        /// <returns>
        /// true if mouse button and modifiers are down.
        /// false if mouse button and modifiers are not down.
        /// </returns>
        bool IsMouseButtonDown(MouseButton button, KeyModifier mods = KeyModifier::None);
        /// <summary>
        /// Check if mouse button and modifiers are not down.
        /// </summary>
        /// <param name="button">Button to check.</param>
        /// <param name="mods">Modifiers to check.</param>
        /// <returns>
        /// true if mouse button and modifiers are not down.
        /// false if mouse button and modifiers are down.
        /// </returns>
        bool IsMouseButtonUp(MouseButton button, KeyModifier mods = KeyModifier::None);

        /// <returns>Scroll offset from the last update call.</returns>
        Vector2<float> GetMouseScrollOffset() const;

        /// <summary>
        /// Set a pointer that the user can access if they have the corresponding window.
        /// </summary>
        /// <param name="ptr">pointer to set.</param>
        void SetUserPointer(void* ptr);
        /// <summary>
        /// Get the pointer the user set. If no pointer was set this value is nullptr.
        /// </summary>
        /// <returns>The pointer the user set.</returns>
        void* GetUserPointer() const;

        /// <summary>
        /// Set the window position to the center of a monitor.
        /// </summary>
        /// <param name="monitor">Monitor that the window will be centered too.</param>
        void Center(Monitor monitor);

        /// <summary>
        /// Set the window to not be in fullscreen or not in fullscreen. 
        /// The window will not be decorated in fullscreen mode.
        /// </summary>
        /// <param name="fullscreen">
        /// Set to true if you want to make the window to fullscreen.
        /// Set to false if you want to make the window not fullscreen.
        /// </param>
        /// <param name="monitor">
        /// The monitor the window be in after setting the window to fullscreen. 
        /// If you not setting the window to fullscreen this window will be in the middle of the monitor provided.
        /// </param>
        void Fullscreen(bool fullscreen, Monitor monitor);
        /// <summary>
        /// Checks if the window is fullscreen.
        /// </summary>
        /// <returns>
        /// true if the window is fullscreen.
        /// false if the window is not fullscreen.
        /// </returns>
        bool IsFullscreen() const;

        /// <summary>
        /// Set the window icon to a custom image. Only applies to decorated windows.
        /// On windows this will set both big and small icons.
        /// </summary>
        /// <param name="image">Image to set the window icon to.</param>
        void SetIcon(Image image);
        /// <summary>
        /// Set the mouse cursor to a custom image.
        /// </summary>
        /// <param name="image">Image to set the mouse cursor to.</param>
        /// <param name="hot">The position of where the cursor clicks according to local space.</param>
        void SetCursor(Image image, Vector2<int32_t> hot);
        /// <summary>
        /// set the window icon to an OS provided icons.
        /// </summary>
        /// <param name="iconID">Reserved.</param>
        void SetIcon(IconID iconID = IconID::Default);
        /// <summary>
        /// Set the cursor to an OS provided cursor.
        /// </summary>
        /// <param name="cursorID">The name of the cursor to set to.</param>
        void SetCursor(CursorID cursorID);
        
        /// <summary>
        /// Get the text currently in the clipboard.
        /// </summary>
        /// <returns>
        /// Returns either the text contained in the clipboard if the method succeeded or an empty std::string if it failed.
        /// </returns>
        std::string GetClipboardText() const;
        /// <summary>
        /// Set the clipboard text to a utf-8 string.
        /// </summary>
        /// <param name="text">String to set the clipboard to.</param>
        void SetClipboardText(const std::string& text);
        
        /// <returns>Time since this window was created.</returns>
        double GetTime() const;

        /// <summary>
        /// Checks if the user is focused on the window.
        /// </summary>
        /// <returns>
        /// true if the window is focused.
        /// false if the window is not fucused.
        /// </returns>
        bool IsFocused() const;
        /// <summary>
        /// Check if the window is iconified or minimized.
        /// </summary>
        /// <returns>
        /// true if the window is iconified.
        /// false if the window is not iconified.
        /// </returns>
        bool IsIconified() const;
        /// <summary>
        /// Check if the window is maximized.
        /// </summary>
        /// <returns>
        /// true if the window is maximized.
        /// false if the window is not maximized.
        /// </returns>
        bool IsMaximized() const;

        /// <summary>
        /// Set the title of the window. Only applies to decorated windows.
        /// </summary>
        /// <param name="title">New title of the window in utf-16 format.</param>
        void SetTitle(const std::wstring& title);
        std::wstring GetTitle() const;
        /// <summary>
        /// Sets the OS's underlying windowing api's window styles.
        /// </summary>
        /// <param name="style">Styles to set.</param>
        void SetStyle(Style style);

        // Input callbacks
        typedef std::function<void(Window&, Key, KeyModifier, InputState, bool)> KeyCallback;
        typedef std::function<void(Window&, Vector2<int32_t>)> MouseMoveCallback;
        typedef std::function<void(Window&, Vector2<float>)> MouseScrollCallback;
        typedef std::function<void(Window&, MouseButton, KeyModifier, InputState)> MouseButtonCallback;
        typedef std::function<void(Window&, bool)> WindowFocusCallback;
        typedef std::function<void(Window&, char32_t, KeyModifier)> CharCallback;
        typedef std::function<void(Window&, std::vector<std::wstring>&, Vector2<int32_t>)> PathDropCallback;
        typedef WindowFocusCallback MouseEnteredCallback;
        typedef WindowFocusCallback WindowIconifiedCallback;
        typedef WindowFocusCallback WindowMaximizedCallback;

        typedef MouseMoveCallback WindowPosCallback;
        typedef WindowPosCallback WindowSizeCallback;
        typedef WindowSizeCallback FramebufferSizeCallback;

        WindowPosCallback SetPosCallback(WindowPosCallback callback);
        WindowSizeCallback SetSizeCallback(WindowSizeCallback callback);
        KeyCallback SetKeyCallback(KeyCallback callback);
        MouseMoveCallback SetMouseMoveCallback(MouseMoveCallback callback);
        MouseButtonCallback SetMouseButtonCallback(MouseButtonCallback callback);
        MouseScrollCallback SetMouseScrollCallback(MouseScrollCallback callback);
        WindowFocusCallback SetWindowFocusCallback(WindowFocusCallback callback);
        MouseEnteredCallback SetMouseEnteredCallback(MouseEnteredCallback callback);
        CharCallback SetCharCallback(CharCallback callback);
        FramebufferSizeCallback SetFramebufferSizeCallback(FramebufferSizeCallback callback);
        WindowIconifiedCallback SetWindowIconifiedCallback(WindowIconifiedCallback callback);
        WindowMaximizedCallback SetWindowMaximizedCallback(WindowMaximizedCallback callback);
        PathDropCallback SetPathDropCallback(PathDropCallback callback);

        void operator=(Window&) = delete;
        Window(Window&) = delete;
        bool operator==(IWindow::Window& window);
        bool operator!=(IWindow::Window& window);
    private:
#if defined (_WIN32)
        LRESULT CALLBACK WindowCallback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);
        static LRESULT CALLBACK s_WindowCallback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);
#endif
        Vector2<int32_t> m_size, m_oldSize, m_position, m_framebufferSize, m_mousePosition;
        Vector2<float> m_scrollOffset;
        // wstring guarantees that the chars are 16 bit not 32 bit which std::wstring does not guarantee.
        std::wstring m_title;

        bool m_fullscreen;
        bool m_running;
        bool m_focused;
        bool m_mouseEntered;
        bool m_iconified;
        bool m_maximized;

        NativeStyle m_windowStyle;

        std::chrono::high_resolution_clock::time_point m_timeMS;

        NativeWindowHandle m_window;

        std::vector<bool> m_keys{ false };
        std::vector<bool> m_keysPressedOnce{ false };
        KeyModifier m_mods;
        std::vector<bool> m_mouseButtons{ false };

        static void DefaultWindowPosCallback(Window&, Vector2<int32_t>) {}
        static void DefaultWindowSizeCallback(Window&, Vector2<int32_t>) {}
        static void DefaultKeyCallback(Window&, Key, KeyModifier, InputState, bool) {}
        static void DefaultMouseMoveCallback(Window&, Vector2<int32_t>) {}
        static void DefaultMouseButtonCallback(Window&, MouseButton, KeyModifier, InputState) {}
        static void DefaultMouseScrollCallback(Window&, Vector2<float>) {}
        static void DefaultWindowFocusCallback(Window&, bool) {}
        static void DefaultMouseEnteredCallback(Window&, bool) {}
        static void DefaultCharCallback(Window&, char32_t, KeyModifier) {}
        static void DefaultFramebufferSizeCallback(Window&, Vector2<int32_t>) {}
        static void DefaultWindowIconifiedCallback(Window&, bool) {}
        static void DefaultWindowMaximizedCallback(Window&, bool) {}
        static void DefualtPathDropCallback(Window&, std::vector<std::wstring>&, Vector2<int32_t>) {}

        WindowPosCallback m_posCallback = DefaultWindowPosCallback;
        WindowSizeCallback m_sizeCallback = DefaultWindowSizeCallback;
        KeyCallback m_keyCallback = DefaultKeyCallback;
        MouseMoveCallback m_mouseMovecallback = DefaultMouseMoveCallback;
        MouseButtonCallback m_mouseButtonCallback = DefaultMouseButtonCallback;
        MouseScrollCallback m_mouseScrollCallback = DefaultMouseScrollCallback;
        WindowFocusCallback m_windowFocusCallback = DefaultWindowFocusCallback;
        MouseEnteredCallback m_mouseEnteredCallback = DefaultMouseEnteredCallback;
        CharCallback m_charCallback = DefaultCharCallback;
        FramebufferSizeCallback m_framebufferSizeCallback = DefaultFramebufferSizeCallback;
        WindowIconifiedCallback m_inconifiedCallback = DefaultWindowIconifiedCallback;
        WindowMaximizedCallback m_maximizedCallback = DefaultWindowMaximizedCallback;
        PathDropCallback m_pathDropCallback = DefualtPathDropCallback;

        NativeDeviceContext m_deviceContext;

        NativeCursor m_cursor;
        NativeIcon m_icon;

        void* m_userPtr;

        static uint32_t m_sWindowCount;
        uint32_t m_windowIndex;

        static Version m_version;
    };
}