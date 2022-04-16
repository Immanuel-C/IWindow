#include "IWindowGamepad.h"

namespace IWindow {
    Gamepad::Gamepad(int gamepadIndex) : m_gamepadIndex { gamepadIndex } {
    }

    XINPUT_STATE Gamepad::GetState() {
        XINPUT_STATE state{};

        ::ZeroMemory(&state, sizeof(XINPUT_STATE));

        XInputGetState(m_gamepadIndex, &state);

        return state;
    }

    int Gamepad::GetIndex() {
        return m_gamepadIndex;
    }

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

    short Gamepad::LeftStickX() { return m_state.Gamepad.sThumbLX; }
    short Gamepad::LeftStickY() { return m_state.Gamepad.sThumbLY; }
    short Gamepad::RightStickX() { return m_state.Gamepad.sThumbRX; }
    short Gamepad::RightStickY() { return m_state.Gamepad.sThumbRY; }

    float Gamepad::LeftTrigger() {
        BYTE trigger = m_state.Gamepad.bLeftTrigger;

        if (trigger >XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
            return trigger;

        return 0.0f;
    }

    float Gamepad::RightTrigger() {
        BYTE trigger = m_state.Gamepad.bRightTrigger;

        if (trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
            return trigger / 255.0f;

        return 0.0f;
    }

    void Gamepad::Rumble(float leftMotor, float rightMotor) {
        XINPUT_VIBRATION vibrationState{};

        ::ZeroMemory(&vibrationState, sizeof(XINPUT_VIBRATION));

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

    void Gamepad::Update() {
        m_state = GetState();
    }

}