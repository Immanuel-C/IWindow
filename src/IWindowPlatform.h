#pragma once

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>

namespace IWindow {
    typedef HWND NativeWindowHandle; 
    typedef HDC NativeDeviceContext;
    typedef HGLRC NativeGLRendereringContext;
    typedef HCURSOR NativeCursor;
    typedef HICON NativeIcon; 

    enum struct NativeCursorID {
        Arrow = (int64_t)IDC_ARROW,
        Hand = (int64_t)IDC_HAND,
        Max
    };

    enum struct NativeIconID {
        Default = (int64_t)IDI_APPLICATION,
        Max
    };

    // Gamepad
    typedef XINPUT_STATE NativeGamepadState;
}
#endif



