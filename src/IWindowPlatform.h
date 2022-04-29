#pragma once

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#include <windowsx.h>
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <GL/glx.h>
#endif

namespace IWindow {
    #if defined(_WIN32)

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
    typedef void* X11Display;
#else
    typedef ::Window NativeWindowHandle;
    typedef Display* X11Display;
    typedef GC NativeDeviceContext;
    typedef GLXContext NativeGLRendereringContext;
    typedef void* NativeCursor;
    typedef void* NativeIcon; 

    enum struct NativeCursorID {
        Max
    };

    enum struct NativeIconID {
        Max
    };
#endif
};


