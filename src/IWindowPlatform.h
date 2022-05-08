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
#include <linux/joystick.h>
#endif


#if __cplusplus >= 202002L
#define IWINDOW_LIKELY [[likely]]
#define IWINDOW_UNLIKELY [[unlikely]]
#else
#define IWINDOW_LIKELY 
#define IWINDOW_UNLIKELY 
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
    typedef js_event NativeGamepadState;
    typedef Cursor NativeCursor;
    typedef void* NativeIcon; 

    enum struct NativeCursorID {
        Arrow,
        Hand,
        Max
    };

    enum struct NativeIconID {
        Default,
        Max
    };
#endif
};


