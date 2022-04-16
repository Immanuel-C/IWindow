#pragma once

#include "IWindowCodes.h"

namespace IWindow {
    class Gamepad {
    public:
        Gamepad() = default;
        Gamepad(int gamepadIndex);

        bool IsLeftStickInDeadzone();
        bool IsRightStickInDeadzone();

        short LeftStickX();
        short LeftStickY();
        short RightStickX();
        short RightStickY();

        float LeftTrigger(); 
        float RightTrigger();

        bool IsButtonDown(GamepadButton button);
        template <typename ... Args>
        bool IsButtonDown(GamepadButton button, Args... args) { return IsButtonDown(button) && IsButtonDown(args...); }

        bool IsButtonUp(GamepadButton button);
        template <typename ... Args>
        bool IsButtonUp(GamepadButton button, Args... args) { return IsButtonUp(button) && IsButtonUp(args...); }

        XINPUT_STATE GetState();
        int GetIndex();
        bool IsConnected();


        // 0.0f = cancel, 1.0f max speed
        void Rumble(float leftMotor = 0.0f, float rightMotor = 0.0f);

        void Update();

    private:
        XINPUT_STATE m_state;
        int m_gamepadIndex;
    };
}