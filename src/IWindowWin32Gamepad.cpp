#ifdef _WIN32

#include "IWindowGamepad.h"

namespace IWindow {
    Gamepad::GamepadConnectedCallback Gamepad::m_connectedCallback = nullptr;

    std::array<bool, (int)GamepadID::Max> Gamepad::m_connectedGamepads{false};

    std::array<void*, (int)GamepadID::Max> Gamepad::m_userPtrs{nullptr};

    Gamepad::Gamepad(GamepadID gamepadIndex) 
    : m_gamepadIndex { (int)gamepadIndex }
    {
        if (!m_connectedCallback)
            m_connectedCallback = DefaultGamepadConnectedCallback;
    }

    XINPUT_STATE Gamepad::GetState() {
        XINPUT_STATE state{};

        // ::ZeroMemory(&state, sizeof(XINPUT_STATE));

        XInputGetState(m_gamepadIndex, &state);

        return state;
    }

    GamepadID Gamepad::GetID() { return (GamepadID)m_gamepadIndex; }

    bool Gamepad::IsConnected() {
        ::ZeroMemory(&m_state, sizeof(XINPUT_STATE));
        DWORD result = XInputGetState(m_gamepadIndex, &m_state);
        return result == ERROR_SUCCESS;
    }

    bool Gamepad::IsLeftStickInDeadzone() {
        short x = m_state.Gamepad.sThumbLX;
        short y = m_state.Gamepad.sThumbLY;

       if (x > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
           x < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
           return false;
 
       if (y > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
           y < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
           return false;

        return true;
    }

    bool Gamepad::IsRightStickInDeadzone() {
        short x = m_state.Gamepad.sThumbRX;
        short y = m_state.Gamepad.sThumbRY;

       if (x > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
           x < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
           return false;
 
       if (y > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
           y < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
           return false;

        return true;
    }

    float Gamepad::LeftStickX() { 
        // sThumb_X_X is a short and the value goes from -SHORT_MAX -> SHORT_MAX
        // but we want a value between -1 and 1 with decimals 
        return m_state.Gamepad.sThumbLX / 32767.0f; 
    }

    float Gamepad::LeftStickY() { return m_state.Gamepad.sThumbLY  / 32767.0f; }
    float Gamepad::RightStickX() { return m_state.Gamepad.sThumbRX / 32767.0f; }
    float Gamepad::RightStickY() { return m_state.Gamepad.sThumbRY / 32767.0f; }

    float Gamepad::LeftTrigger() {
        BYTE trigger = m_state.Gamepad.bLeftTrigger;

        if (trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
            return trigger;

        return 0.0f;
    }

    float Gamepad::RightTrigger() {
        BYTE trigger = m_state.Gamepad.bRightTrigger;

        if (trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
            // Range is usually 0 - 255 but I want 0, 0.1, 0.2, ... 1
            // so dividing by 255 gets me this
            return trigger / 255.0f;

        return 0.0f;
    }

    void Gamepad::Rumble(float leftMotor, float rightMotor) {
        XINPUT_VIBRATION vibrationState{};

        // ::ZeroMemory(&vibrationState, sizeof(XINPUT_VIBRATION));

        // calculate real XInput rumble values
        int iLeftMotor = int(leftMotor * 65535.0f);
        int iRightMotor = int(rightMotor * 65535.0f);

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