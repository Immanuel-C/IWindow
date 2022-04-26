#pragma once

#if defined(_WIN32)
#include <windows.h>
#include <Xinput.h>
#include <windowsx.h>
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#endif

namespace IWindow {
    #if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN

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
#else
    typedef ::Window NativeWindowHandle;
#endif
    typedef void* X11Display;
    typedef void* NativeDeviceContext;
};


