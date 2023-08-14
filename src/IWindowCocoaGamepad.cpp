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
#if defined(__APPLE__)

#include "IWindowGamepad.h"

namespace IWindow {
    GamepadConnectedCallback Gamepad::m_connectedCallback = DefaultGamepadConnectedCallback;

    std::array<bool, (int)GamepadID::Max> Gamepad::m_connectedGamepads{false};

    std::array<void*, (int)GamepadID::Max> Gamepad::m_userPtrs{nullptr};

    Gamepad::Gamepad(GamepadID gamepadIndex) 
    : m_gamepadIndex { (int)gamepadIndex }
    {

    }

    Gamepad::~Gamepad() { }

    GamepadID Gamepad::GetID() { return (GamepadID)m_gamepadIndex; }

    bool Gamepad::IsLeftStickInDeadzone() {
        //TODO

        return true;
    }

    bool Gamepad::IsRightStickInDeadzone() {
        //TODO

        return true;
    }

    float Gamepad::LeftStickX() { return 0.0f; } //TODO
    float Gamepad::LeftStickY() { return 0.0f; } //TODO
    float Gamepad::RightStickX() { return 0.0f; } //TODO
    float Gamepad::RightStickY() { return 0.0f; } //TODO

    float Gamepad::LeftTrigger() {
        //TODO

        return 0.0f;
    }

    float Gamepad::RightTrigger() {
        //TODO

        return 0.0f;
    }

    void Gamepad::Rumble(float leftMotor, float rightMotor) {
        //TODO

    }

    bool Gamepad::IsButtonDown(GamepadButton button) { return false; } //TODO
    bool Gamepad::IsButtonUp(GamepadButton button) { return !IsButtonDown(button); }

    void Gamepad::SetConnectedCallback(GamepadConnectedCallback callback) { m_connectedCallback = callback; }

    void Gamepad::SetUserPointer(GamepadID gid, void* ptr) { m_userPtrs[(int)gid] = ptr; }

    void* Gamepad::GetUserPointer(GamepadID gid) { return m_userPtrs[(int)gid]; }

    void Gamepad::Update() { 
        //TODO
    }
}
#endif
