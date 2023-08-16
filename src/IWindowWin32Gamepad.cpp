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
#if defined(_WIN32)

#include "IWindowGamepad.h"

namespace IWindow {
    GamepadConnectedCallback Gamepad::m_connectedCallback = DefaultGamepadConnectedCallback;

    std::array<bool, (int)GamepadID::Max> Gamepad::m_connectedGamepads{false};

    std::array<void*, (int)GamepadID::Max> Gamepad::m_userPtrs{nullptr};

    Gamepad::Gamepad(GamepadID gamepadIndex, float triggerDeadzone, float stickDeadzone) 
    : m_gamepadIndex { (int)gamepadIndex },
      m_triggerDeadzone { triggerDeadzone },
      m_stickDeadzone { stickDeadzone }
    {
    }

    Gamepad::~Gamepad() { }

    XINPUT_STATE Gamepad::GetState() {
        XINPUT_STATE state{};

        ::ZeroMemory(&state, sizeof(XINPUT_STATE));

        XInputGetState(m_gamepadIndex, &state);

        return state;
    }

    GamepadID Gamepad::GetID() { return (GamepadID)m_gamepadIndex; }

    bool Gamepad::IsConnected() {
        ::ZeroMemory(&m_state, sizeof(XINPUT_STATE));
        DWORD result = XInputGetState(m_gamepadIndex, &m_state);
        return result == ERROR_SUCCESS;
    }

    float Gamepad::LeftStickX() { 
        // sThumb_X_X is a short and the value goes from -SHORT_MAX -> SHORT_MAX
        // but we want a value between -1 and 1 with decimals 
        float leftThumbstickX = (float)m_state.Gamepad.sThumbLX;
        leftThumbstickX /= SHRT_MAX;

        // Check if values are in deadzone
        if (leftThumbstickX > m_stickDeadzone || leftThumbstickX < -m_stickDeadzone)
            return leftThumbstickX;

        return 0;
    }

    float Gamepad::LeftStickY() {
        float leftThumbstickY = (float)m_state.Gamepad.sThumbLY;
        leftThumbstickY /= SHRT_MAX;

        if (leftThumbstickY > m_stickDeadzone || leftThumbstickY < -m_stickDeadzone)
            return leftThumbstickY;

        return 0;
    }

    float Gamepad::RightStickX() {
        float rightThumbstickX = (float)m_state.Gamepad.sThumbRX;
        rightThumbstickX /= SHRT_MAX;

        if (rightThumbstickX > m_stickDeadzone || rightThumbstickX < -m_stickDeadzone)
            return rightThumbstickX;

        return 0;
    }
    float Gamepad::RightStickY() {
        float rightThumbstickY = (float)m_state.Gamepad.sThumbRY;
        rightThumbstickY /= SHRT_MAX;

        if (rightThumbstickY > m_stickDeadzone || rightThumbstickY < -m_stickDeadzone)
            return rightThumbstickY;

        return 0;
    }

    float Gamepad::LeftTrigger() {
        float trigger = (float)m_state.Gamepad.bLeftTrigger;
        // Range is usually 0 - 255 we want the range to be 0 - 1
        trigger /= 255.0f;

        if (trigger > m_triggerDeadzone)
            return trigger;

        return 0.0f;
    }

    float Gamepad::RightTrigger() {
        float trigger = (float)m_state.Gamepad.bRightTrigger;
        trigger /= 255.0f;

        if (trigger > m_triggerDeadzone)
            return trigger;

        return 0.0f;
    }

    void Gamepad::Rumble(float rumble) {
        XINPUT_VIBRATION vibrationState{};

        ::ZeroMemory(&vibrationState, sizeof(XINPUT_VIBRATION));

        // calculate real XInput rumble values
        int iLeftMotor = int(rumble * 65535.0f);
        int iRightMotor = int(rumble * 65535.0f);

        // Set vibration values
        vibrationState.wLeftMotorSpeed  = iLeftMotor;
        vibrationState.wRightMotorSpeed = iRightMotor;
    
        // Set the vibration state
        XInputSetState(m_gamepadIndex, &vibrationState);

    }

    bool Gamepad::IsButtonDown(GamepadButton button) { return m_state.Gamepad.wButtons & (int)button; }
    bool Gamepad::IsButtonUp(GamepadButton button) { return !IsButtonDown(button); }

    void Gamepad::SetConnectedCallback(GamepadConnectedCallback callback) { m_connectedCallback = callback; }

    void Gamepad::SetUserPointer(GamepadID gid, void* ptr) { m_userPtrs[(int)gid] = ptr; }

    void* Gamepad::GetUserPointer(GamepadID gid) { return m_userPtrs[(int)gid]; }

    void Gamepad::SetTriggerDeadzone(float deadzone) { m_triggerDeadzone = deadzone; }
    float Gamepad::GetTriggerDeadzone() { return m_triggerDeadzone; }

    void Gamepad::SetStickDeadzone(float deadzone) { m_stickDeadzone = deadzone; }

    float Gamepad::GetStickDeadzone() { return m_stickDeadzone; }

    void Gamepad::Update() { 
        for (uint32_t i = 0; i < (uint32_t)GamepadID::Max; i++) {
            DWORD result = XInputGetState(i, &m_state);
            // Connected
            if (result == ERROR_SUCCESS && !m_connectedGamepads[i]) {
                m_connectedCallback((GamepadID)i, true);
                m_connectedGamepads[i] = true;
            } 
            // Disconnected
            else if (result != ERROR_SUCCESS && m_connectedGamepads[i]) {
                m_connectedCallback((GamepadID)i, false);
                m_connectedGamepads[i] = false;
            }

        }

        ::ZeroMemory(&m_state, sizeof(XINPUT_STATE));

        m_state = GetState(); 
    }
}
#endif
