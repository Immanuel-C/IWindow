#pragma once

#include <array>
#include <functional>

#include "IWindowCodes.h"
#include "IWindowPlatform.h"

namespace IWindow {
    class Gamepad {
    private:
        typedef std::function<void(GamepadID, bool)> GamepadConnectedCallback;
    public:
        Gamepad() = default;
        Gamepad(GamepadID gamepadIndex);

        bool IsLeftStickInDeadzone();
        bool IsRightStickInDeadzone();

        float LeftStickX();
        float LeftStickY();
        float RightStickX();
        float RightStickY();

        float LeftTrigger(); 
        float RightTrigger();

        bool IsButtonDown(GamepadButton button);
        template <typename ... Args>
        bool IsButtonDown(GamepadButton button, Args... args) { return IsButtonDown(button) && IsButtonDown(args...); }

        bool IsButtonUp(GamepadButton button);
        template <typename ... Args>
        bool IsButtonUp(GamepadButton button, Args... args) { return IsButtonUp(button) && IsButtonUp(args...); }

        GamepadState GetState();
        GamepadID GetID();
        bool IsConnected();

        static void SetConnectedCallback(GamepadConnectedCallback callback);

        static void SetUserPointer(GamepadID gid, void* ptr);
        static void* GetUserPointer(GamepadID gid);

        // 0.0f = cancel, 1.0f max speed
        void Rumble(float leftMotor = 0.0f, float rightMotor = 0.0f);

        void Update();

    private:
        GamepadState m_state;
        int m_gamepadIndex;

        static void DefaultGamepadConnectedCallback(GamepadID, bool) {}

        static GamepadConnectedCallback m_connectedCallback;

        static std::array<bool, (int)GamepadID::Max> m_connectedGamepads;

        static std::array<void*, (int)GamepadID::Max> m_userPtrs;
    };
}