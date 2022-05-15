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

#include <array>
#include <functional>
#include <string>
#include <future>

#include "IWindowCodes.h"
#include "IWindowPlatform.h"

namespace IWindow {
    typedef std::function<void(GamepadID, bool)> GamepadConnectedCallback;

    class Gamepad {
    public:
        Gamepad() = default;
        Gamepad(GamepadID gamepadIndex);
        ~Gamepad();


        // TODO(Immu): Allow user defined deadzone
        bool IsLeftStickInDeadzone();
        // TODO(Immu): Allow user defined deadzone
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

        NativeGamepadState GetState();
        GamepadID GetID();
        bool IsConnected();

        static void SetConnectedCallback(GamepadConnectedCallback callback);

        static void SetUserPointer(GamepadID gid, void* ptr);
        static void* GetUserPointer(GamepadID gid);

        /*
            0.0f = cancel, 1.0f max speed
            Windows only for now
        */
        void Rumble(float leftMotor = 0.0f, float rightMotor = 0.0f);

        void Update();
    private:
        NativeGamepadState m_state;
        int m_gamepadIndex;

        static void DefaultGamepadConnectedCallback(GamepadID, bool) {}

        static GamepadConnectedCallback m_connectedCallback;

        static std::array<bool, (int)GamepadID::Max> m_connectedGamepads;

        static std::array<void*, (int)GamepadID::Max> m_userPtrs;

        // Linux Only
        std::string m_devPath, m_devEventPath;
        int m_js, m_event;
        std::future<void> m_rumbleFuture;
    };
    
}