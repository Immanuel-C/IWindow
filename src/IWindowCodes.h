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

#endif // _WIN32