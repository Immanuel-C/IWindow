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

#include "IWindowUtils.h"

#if defined (_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h> // For virtual key codes 
    #include <Xinput.h> // xinput codes

namespace IWindow {
    /// <summary>
    /// Represents keyboard key's.
    /// </summary>
    enum struct Key {

        // Numbers dont have virtual key codes
        N0,
        N1,
        N2,
        N3,
        N4,
        N5,
        N6,
        N7,
        N8,
        N9,

        // Alphabet dont have virtual key codes
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        Back,
        Tab,
        Enter,
        Pause,
        Escape,
        Space,
        PageUp,
        PageDown,
        End,
        Home,
        ArrowLeft,
        ArrowUp,
        ArrowRight,
        ArrowDown,
        PrintScreen,
        Insert,
        Delete,
        Add,
        Multiply,
        Subtract,
        Decimal,
        Divide,
        ScrollLock,
        SemiColen,
        Plus,
        Comma,
        Minus,
        Period,
        ForwardSlash,
        Tilde,
        LeftBoxBraces,
        BackSlash,
        RightBoxBraces,
        SingleQuotes,

        LControl,
        RControl,
        LShift,
        RShift,
        LAlt,
        RAlt,
        LSuper,
        RSuper,
        CapsLock,
        NumLock,

        Numpad0,
        Numpad1,
        Numpad2,
        Numpad3,
        Numpad4,
        Numpad5,
        Numpad6,
        Numpad7,
        Numpad8,
        Numpad9,

        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        Max
    };

    /// <summary>
    /// Convert Virtual Key codes to IWindow::Key.
    /// </summary>
    inline Key Win32KeyCodeToIWindowKey(uint64_t key) {
        switch (key)
        {
        case 48:
            return Key::N0;
            break;
        case 49:
            return Key::N1;
            break;
        case 50:
            return Key::N2;
            break;
        case 51:
            return Key::N3;
            break;
        case 52:
            return Key::N4;
            break;
        case 53:
            return Key::N5;
            break;
        case 54:
            return Key::N6;
            break;
        case 55:
            return Key::N7;
            break;
        case 56:
            return Key::N8;
            break;
        case 57:
            return Key::N9;
            break;
        case 65:
            return Key::A;
            break;
        case 66:
            return Key::B;
            break;
        case 67:
            return Key::C;
            break;
        case 68:
            return Key::D;
            break;
        case 69:
            return Key::E;
            break;
        case 70:
            return Key::F;
            break;
        case 71:
            return Key::G;
            break;
        case 72:
            return Key::H;
            break;
        case 73:
            return Key::I;
            break;
        case 74:
            return Key::J;
            break;
        case 75:
            return Key::K;
            break;
        case 76:
            return Key::L;
            break;
        case 77:
            return Key::M;
            break;
        case 78:
            return Key::N;
            break;
        case 79:
            return Key::O;
            break;
        case 80:
            return Key::P;
            break;
        case 81:
            return Key::Q;
            break;
        case 82:
            return Key::R;
            break;
        case 83:
            return Key::S;
            break;
        case 84:
            return Key::T;
            break;
        case 85:
            return Key::U;
            break;
        case 86:
            return Key::V;
            break;
        case 87:
            return Key::W;
            break;
        case 88:
            return Key::X;
            break;
        case 89:
            return Key::Y;
            break;
        case 90:
            return Key::Z;
            break;
        case VK_BACK:
            return Key::Back;
            break;
        case VK_TAB:
            return Key::Tab;
            break;
        case VK_RETURN:
            return Key::Enter;
            break;
        case VK_PAUSE:
            return Key::Pause;
            break;
        case VK_ESCAPE:
            return Key::Escape;
            break;
        case VK_SPACE:
            return Key::Space;
            break;
        case VK_PRIOR:
            return Key::PageUp;
            break;
        case VK_NEXT:
            return Key::PageDown;
            break;
        case VK_END:
            return Key::End;
            break;
        case VK_HOME:
            return Key::Home;
            break;
        case VK_LEFT:
            return Key::ArrowLeft;
            break;
        case VK_UP:
            return Key::ArrowUp;
            break;
        case VK_RIGHT:
            return Key::ArrowRight;
            break;
        case VK_DOWN:
            return Key::ArrowDown;
            break;
        case VK_SNAPSHOT:
            return Key::PrintScreen;
            break;
        case VK_INSERT:
            return Key::Insert;
            break;
        case VK_DELETE:
            return Key::Delete;
            break;
        case VK_ADD:
            return Key::Add;
            break;
        case VK_MULTIPLY:
            return Key::Multiply;
            break;
        case VK_SUBTRACT:
            return Key::Subtract;
            break;
        case VK_DECIMAL:
            return Key::Decimal;
            break;
        case VK_DIVIDE:
            return Key::Divide;
            break;
        case VK_SCROLL:
            return Key::ScrollLock;
            break;
        case VK_OEM_1:
            return Key::SemiColen;
            break;
        case VK_OEM_PLUS:
            return Key::Plus;
            break;
        case VK_OEM_COMMA:
            return Key::Comma;
            break;
        case VK_OEM_MINUS:
            return Key::Minus;
            break;
        case VK_OEM_PERIOD:
            return Key::Period;
            break;
        case VK_OEM_2:
            return Key::ForwardSlash;
            break;
        case VK_OEM_3:
            return Key::Tilde;
            break;
        case VK_OEM_4:
            return Key::LeftBoxBraces;
            break;
        case VK_OEM_5:
            return Key::BackSlash;
            break;
        case VK_OEM_6:
            return Key::RightBoxBraces;
            break;
        case VK_OEM_7:
            return Key::SingleQuotes;
            break;
        case VK_NUMPAD0:
            return Key::Numpad0;
            break;
        case VK_NUMPAD1:
            return Key::Numpad1;
            break;
        case VK_NUMPAD2:
            return Key::Numpad2;
            break;
        case VK_NUMPAD3:
            return Key::Numpad3;
            break;
        case VK_NUMPAD4:
            return Key::Numpad4;
            break;
        case VK_NUMPAD5:
            return Key::Numpad5;
            break;
        case VK_NUMPAD6:
            return Key::Numpad6;
            break;
        case VK_NUMPAD7:
            return Key::Numpad7;
            break;
        case VK_NUMPAD8:
            return Key::Numpad8;
            break;
        case VK_NUMPAD9:
            return Key::Numpad9;
            break;
        case VK_F1:
            return Key::F1;
            break;
        case VK_F2:
            return Key::F2;
            break;
        case VK_F3:
            return Key::F3;
            break;
        case VK_F4:
            return Key::F4;
            break;
        case VK_F5:
            return Key::F5;
            break;
        case VK_F6:
            return Key::F6;
            break;
        case VK_F7:
            return Key::F7;
            break;
        case VK_F8:
            return Key::F8;
            break;
        case VK_F9:
            return Key::F9;
            break;
        case VK_F10:
            return Key::F10;
            break;
        case VK_F11:
            return Key::F11;
            break;
        case VK_F12:
            return Key::F12;
            break;
        case VK_LCONTROL:
            return Key::LControl;
        case VK_RCONTROL:
            return Key::RControl;
        case VK_LMENU:
            return Key::LAlt;
        case VK_RMENU:
            return Key::RAlt;
        case VK_LSHIFT:
            return Key::LShift;
        case VK_RSHIFT:
            return Key::RShift;
        case VK_LWIN:
            return Key::LSuper;
        case VK_RWIN:
            return Key::RSuper;
        case VK_CAPITAL:
            return Key::CapsLock;
        case VK_NUMLOCK:
            return Key::NumLock;
        default:
            // key code not supported by IWindow
            return Key::Max;
            break;
        }
    }

    /// <summary>
    /// Represents keyboard modifiers. This is bitmask type.
    /// </summary>
    enum struct KeyModifier {
        None = 0x0,
        Shift = 0x1,
        Control = 0x2,
        Alt = 0x4,
        Super = 0x8,
        CapsLock = 0x10,
        NumLock = 0x20,
        Max
    };
    IWINDOW_CREATE_FLAGS_FROM_ENUM_STRUCT(KeyModifier)

    /// <summary>
    /// Represents mouse buttons.
    /// </summary>
    enum struct MouseButton {
        Left,
        Right,
        Middle, // Scroll Wheel
        Side1,  // Browser back button
        Side2,  // Browser forward button
        Max
    };

    /// <summary>
    /// Represents what state the key/button/modifier was in when it was pressed.
    /// </summary>
    enum struct InputState {
        Down,
        Up,
        Max
    };

    /// <summary>
    /// Represents buttons on a gamepad.
    /// </summary>
    enum struct GamepadButton {
        A = XINPUT_GAMEPAD_A,
        B = XINPUT_GAMEPAD_B,
        X = XINPUT_GAMEPAD_X,
        Y = XINPUT_GAMEPAD_Y,

        DpadUp = XINPUT_GAMEPAD_DPAD_UP,
        DpadDown = XINPUT_GAMEPAD_DPAD_DOWN,
        DpadRight = XINPUT_GAMEPAD_DPAD_RIGHT,
        DpadLeft = XINPUT_GAMEPAD_DPAD_LEFT,

        LeftShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER,
        RightShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER,
        
        LeftStick = XINPUT_GAMEPAD_LEFT_THUMB,
        RightStick = XINPUT_GAMEPAD_RIGHT_THUMB,

        Start = XINPUT_GAMEPAD_START,
        Back = XINPUT_GAMEPAD_BACK,

        Max,
    };


    /// <summary>
    /// ID of gamepads.
    /// </summary>
    enum struct GamepadID {
        GP0,
        GP1,
        GP2,
        GP3,
        
        Max,
    };
}

#else

namespace IWindow {

    enum struct Key {
        N0 = 19,
        N1 = 10,
        N2 = 11,
        N3 = 12,
        N4 = 13,
        N5 = 14,
        N6 = 15,
        N7 = 16,
        N8 = 17,
        N9 = 18,

        A = 38,
        B = 56,
        C = 54,
        D = 40,
        E = 26,
        F = 41,
        G = 42,
        H = 43,
        I = 31,
        J = 44,
        K = 45,
        L = 46,
        M = 58,
        N = 57,
        O = 32,
        P = 33,
        Q = 24,
        R = 27,
        S = 39,
        T = 28,
        U = 30,
        V = 55,
        W = 25,
        X = 53,
        Y = 29,
        Z = 52,

        Np0 = 90,
        Np1 = 87,
        Np2 = 88,
        Np3 = 89,
        Np4 = 83,
        Np5 = 84,
        Np6 = 85,
        Np7 = 79,
        Np8 = 80,
        Np9 = 81,


        F1 = 67,
        F2 = 68,
        F3 = 69,
        F4 = 70,
        F5 = 71,
        F6 = 72,
        F7 = 73,
        F8 = 74,
        F9 = 75,
        F10 = 76,
        F11 = 95,
        F12 = 96,

        Back = 22,
        Tab = 23,
        Enter = 108, 
        Shift = 56,
        Control = 37,
        Alt = 64,
        Pause = 110,
        CapsLock = 66,
        Escape = 9,
        Space = 65,
        PageUp = 99,
        PageDown = 105,
        End = 103,
        Home = 97,
        ArrowLeft = 113,
        ArrowUp = 111,
        ArrowRight = 114,
        ArrowDown = 116,
        PrintScreen = 111,
        Insert = 106,
        Delete = 107,
        LeftSuper = 133,
        RightSuper = 0, // Get this
        Subtract = 20,
        Decimal = 60,
        Add = 86,
        Divide = 61,
        Multiply = 63,
        ScrollLock = 78,
        NumLock = 77,
        LShift = Shift,
        RShift = 62,
        LControl = Control,
        RControl = 109,
        LMenu = 117,
        RMenu = 117,

        Max = 200,
    };


    enum struct MouseButton {
        Left = 1,
        Right = 3,  
        Middle = 2, // Scroll Wheel
        Side1 = 8,  // Browser back button
        Side2 = 9,  // Browser forward button
        Max
    };

    enum struct InputState {
        Down,
        Up,
        Max
    };

    enum struct GamepadButton {
        A,
        B,
        X,
        Y,

        LeftShoulder,
        RightShoulder,

        Back,
        Start,

        //Super,

        LeftStick = 9,
        RightStick = 10,

        DpadUp = 11,
        DpadDown = 12,
        DpadRight = 13,
        DpadLeft = 14,

        Max,
    };

    enum struct GamepadID {
        GP1,
        GP2,
        GP3,
        GP4,
        
        Max,
    };
};
#endif
