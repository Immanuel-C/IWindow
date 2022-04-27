#pragma once

#if defined (_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h> // For virtual key codes 
    #include <Xinput.h> // xinput codes

namespace IWindow {
    enum struct Key {

        // Numbers dont have virtual key codes
        N0 = 48,
        N1 = 49,
        N2 = 50,
        N3 = 51,
        N4 = 52,
        N5 = 53,
        N6 = 54,
        N7 = 55,
        N8 = 56,
        N9 = 57,

        // Alphabet dont have virtual key codes
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,

        Back = VK_BACK,
        Tab = VK_TAB,
        Clear = VK_CLEAR,
        Enter = VK_RETURN,
        Shift = VK_SHIFT,
        Control = VK_CONTROL,
        Alt = VK_MENU,
        Pause = VK_PAUSE,
        CapsLock = VK_CAPITAL,
        Escape = VK_ESCAPE,
        Space = VK_SPACE,
        PageUp = VK_PRIOR,
        PageDown = VK_NEXT,
        End = VK_END,
        Home = VK_HOME,
        ArrowLeft = VK_LEFT,
        ArrowUp = VK_UP,
        ArrowRight = VK_RIGHT,
        ArrowDown = VK_DOWN,
        Select = VK_SELECT,
        Print = VK_PRINT,
        Execute = VK_EXECUTE,
        PrintScreen = VK_SNAPSHOT,
        Insert = VK_INSERT,
        Delete = VK_DELETE,
        Help = VK_HELP,
        LeftSuper = VK_LWIN,
        RightSuper = VK_RWIN,
        Sleep = VK_SLEEP,
        Multiply = VK_MULTIPLY,
        Add = VK_ADD,
        Subtract = VK_SUBTRACT,
        Decimal = VK_DECIMAL,
        Divide = VK_DIVIDE,
        ScrollLock = VK_SCROLL,
        NumLock = VK_NUMLOCK,
        LShift = VK_LSHIFT,
        RShift = VK_RSHIFT,
        LControl = VK_LCONTROL,
        RControl = VK_RCONTROL,
        LMenu = VK_LMENU,
        RMenu = VK_RMENU,


        Np0 = VK_NUMPAD0,
        Np1 = VK_NUMPAD1,
        Np2 = VK_NUMPAD2,
        Np3 = VK_NUMPAD3,
        Np4 = VK_NUMPAD4,
        Np5 = VK_NUMPAD5,
        Np6 = VK_NUMPAD6,
        Np7 = VK_NUMPAD7,
        Np8 = VK_NUMPAD8,
        Np9 = VK_NUMPAD9,

        F1 = VK_F1,
        F2 = VK_F2,
        F3 = VK_F3,
        F4 = VK_F4,
        F5 = VK_F5,
        F6 = VK_F6,
        F7 = VK_F7,
        F8 = VK_F8,
        F9 = VK_F9,
        F10 = VK_F10,
        F11 = VK_F11,
        F12 = VK_F12,

        Max = 500
    };

    enum struct MouseButton {
        Left,
        Right,
        Middle,
        Side1,
        Side2,
        Max
    };

    enum struct InputState {
        Down,
        Up,
        Max
    };

    enum struct ClickState {
        Double,
        Single,
        Up,
        Max
    };

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

    enum struct GamepadID {
        GP1,
        GP2,
        GP3,
        GP4,
        
        Max,
    };
}

#else
namespace IWindow {
    enum struct Key {
        N0 = 0x13,
        N1 = 0xa,
        N2 = 0xb,
        N3 = 0xc,
        N4 = 0xd,
        N5 = 0xe,
        N6 = 0xf,
        N7 = 0x10,
        N8 = 0x11,
        N9 = 0x12,

        A = 0x26,
        B = 0x38,
        C = 0x36,
        D = 0x28,
        E = 0x1a,
        F = 0x29,
        G = 0x2a,
        H = 0x2b,
        I = 0x1f,
        J = 0x2c,
        K = 0x2d,
        L = 0x2e,
        M = 0x3a,
        N = 0x39,
        O = 0x20,
        P = 0x21,
        Q = 0x18,
        R = 0x1b,
        S = 0x27,
        T = 0x1c,
        U = 0x1e,
        V = 0x37,
        W = 0x19,
        X = 0x35,
        Y = 0x1d,
        Z = 0x34,

        Np0 = 0x5a,
        Np1 = 0x57,
        Np2 = 0x58,
        Np3 = 0x59,
        Np4 = 0x53,
        Np5 = 0x54,
        Np6 = 0x55,
        Np7 = 0x4f,
        Np8 = 0x50,
        Np9 = 0x51,

        F1 = 0x43,
        F2 = 0x44,
        F3 = 0x45,
        F4 = 0x46,
        F5 = 0x47,
        F6 = 0x48,
        F7 = 0x49,
        F8 = 0x4a,
        F9 = 0x4b,
        F10 = 0x4c,
        F11 = 0x5f,
        F12 = 0x60,

        Max = 0x61
    };

    inline bool IWindowCheckX11KeySupported(Key key)
    {
        switch (key) {
            case Key::N0: {return true; }
            case Key::N1: {return true; }
            case Key::N2: {return true; }
            case Key::N3: {return true; }
            case Key::N4: {return true; }
            case Key::N5: {return true; }
            case Key::N6: {return true; }
            case Key::N7: {return true; }
            case Key::N8: {return true; }
            case Key::N9: {return true; }


            case Key::A: {return true; }
            case Key::B: {return true; }
            case Key::C: {return true; }
            case Key::D: {return true; }
            case Key::E: {return true; }
            case Key::F: {return true; }
            case Key::G: {return true; }
            case Key::H: {return true; }
            case Key::I: {return true; }
            case Key::J: {return true; }
            case Key::K: {return true; }
            case Key::L: {return true; }
            case Key::M: {return true; }
            case Key::N: {return true; }
            case Key::O: {return true; }
            case Key::P: {return true; }
            case Key::Q: {return true; }
            case Key::R: {return true; }
            case Key::S: {return true; }
            case Key::T: {return true; }
            case Key::U: {return true; }
            case Key::V: {return true; }
            case Key::W: {return true; }
            case Key::X: {return true; }
            case Key::Y: {return true; }
            case Key::Z: {return true; }

            case Key::Np0: {return true; }
            case Key::Np1: {return true; }
            case Key::Np2: {return true; }
            case Key::Np3: {return true; }
            case Key::Np4: {return true; }
            case Key::Np5: {return true; }
            case Key::Np6: {return true; }
            case Key::Np7: {return true; }
            case Key::Np8: {return true; }
            case Key::Np9: {return true; }

            case Key::F1: {return true; }
            case Key::F2: {return true; }
            case Key::F3: {return true; }
            case Key::F4: {return true; }
            case Key::F5: {return true; }
            case Key::F6: {return true; }
            case Key::F7: {return true; }
            case Key::F8: {return true; }
            case Key::F9: {return true; }
            case Key::F10: {return true; }
            case Key::F11: {return true; }
            case Key::F12: {return true; }
        };

        return false;
    }

    enum struct MouseButton {
        Left,
        Right,
        Middle,
        Side1,
        Side2,
        Max
    };

     enum struct InputState {
        Down,
        Up,
        Max
    };


    enum struct ClickState {
        Double,
        Single,
        Up,
        Max
    };

    enum struct GamepadButton {
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