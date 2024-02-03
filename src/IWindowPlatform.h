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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#include <windowsx.h>


namespace IWindow {
    typedef HWND NativeWindowHandle; 
    typedef HDC NativeDeviceContext;
    typedef HGLRC NativeGLRendereringContext;
    typedef HCURSOR NativeCursor;
    typedef HICON NativeIcon; 
    typedef int32_t NativeStyle;

    enum struct CursorID {
        Arrow,
        IBeam,
        Hand,
        Busy,
        BusyBackground,
        DiagonalResize1,
        DiagonalResize2,
        HorizontalResize,
        VerticalResize,
        Move,
        No,
        Hidden,
        Max
    };

    inline wchar_t* CursorIDToWin32ID(CursorID cursorID) {
        switch (cursorID)
        {
        case IWindow::CursorID::Arrow:
            return IDC_ARROW;
        case IWindow::CursorID::IBeam:
            return IDC_IBEAM;
        case IWindow::CursorID::Hand:
            return IDC_HAND;
        case IWindow::CursorID::Busy:
            return IDC_WAIT;
        case IWindow::CursorID::BusyBackground:
            return IDC_APPSTARTING;
        case IWindow::CursorID::DiagonalResize1:
            return IDC_SIZENWSE;
        case IWindow::CursorID::DiagonalResize2:
            return IDC_SIZENESW;
        case IWindow::CursorID::HorizontalResize:
            return IDC_SIZEWE;
        case IWindow::CursorID::VerticalResize:
            return IDC_SIZENS;
        case IWindow::CursorID::Move:
            return IDC_SIZEALL;
        case IWindow::CursorID::No:
            return IDC_NO;
        default:
            return nullptr;
        }
    }

    enum struct IconID {
        Default = (int64_t)IDI_APPLICATION,
        Max = 2
    };


    // Gamepad
    typedef XINPUT_STATE NativeGamepadState;
};


