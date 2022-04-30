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
        PrintScreen = VK_SNAPSHOT,
        Insert = VK_INSERT,
        Delete = VK_DELETE,
        LeftSuper = VK_LWIN,
        RightSuper = VK_RWIN,
        Add = VK_ADD,
        Multiply = VK_MULTIPLY,
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
        Middle, // Scroll Wheel
        Side1,  // Browser back button
        Side2,  // Browser forward button
        Max
    };

    enum struct InputState {
        Down,
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
        ArrowDown = 39,
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

        Super,

        LeftStick,
        RightStick,

        DpadUp,
        DpadDown,
        DpadRight,
        DpadLeft,

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