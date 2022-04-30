#include "IWindowGamepad.h"

#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <iostream>
#include <limits>


namespace IWindow {
    std::string Gamepad::m_devPath = "/dev/input/js1";
    std::array<bool, (int)GamepadID::Max> Gamepad::m_connectedGamepads{ false };
    std::array<void*, (int)GamepadID::Max> Gamepad::m_userPtrs{ nullptr };

    static constexpr int32_t GAMEPAD_AXIS_LEFT_STICK_Y = 0;
    static constexpr int32_t GAMEPAD_AXIS_LEFT_STICK_X = 1;
    static constexpr int32_t GAMEPAD_AXIS_RIGHT_STICK_Y = 3;
    static constexpr int32_t GAMEPAD_AXIS_RIGHT_STICK_X = 4;
    
    static constexpr int32_t GAMEPAD_AXIS_LEFT_TRIGGER = 2;
    static constexpr int32_t GAMEPAD_AXIS_RIGHT_TRIGGER = 5;

    static constexpr int32_t GAMEPAD_AXIS_DPAD_X = 6;
    static constexpr int32_t GAMEPAD_AXIS_DPAD_Y = 7;

    void Gamepad::LinuxSetDevPath(const std::string& devPath) { m_devPath = devPath; }

    Gamepad::Gamepad(GamepadID gamepadIndex) : m_gamepadIndex { (int)gamepadIndex }{
        m_js = open(m_devPath.c_str(), O_RDONLY);
        // -1 if failed
        if (m_js == -1)  std::cout << "Failed to open: \"" << m_devPath << "\" for gamepad input!\n";
    } 

    Gamepad::~Gamepad() {
        if (!IsConnected())
            return;
            
        close(m_js);
    }

    float Gamepad::LeftStickX() {
        if (!IsConnected())
            return 0.0f;
            
        if (m_state.type != JS_EVENT_AXIS && m_state.number != GAMEPAD_AXIS_LEFT_STICK_X)
            return 0.0f;

        // The range is usally between std::numeric_limits<signed short>::max() and std::numeric_limits<signed short>::min()
        // but we want it to be in range -1.0f and 1.0f for consistency 
        return m_state.value / (float)std::numeric_limits<signed short>::max();
    }

    float Gamepad::LeftStickY() {
        if (!IsConnected())
            return 0.0f;
            
        if (m_state.type != JS_EVENT_AXIS && m_state.number != GAMEPAD_AXIS_LEFT_STICK_Y)
            return 0.0f;

        return (float)m_state.value / (float)std::numeric_limits<signed short>::max();
    }

    float Gamepad::RightStickX() {
        if (!IsConnected())
            return 0.0f;
            
        if (m_state.type != JS_EVENT_AXIS && m_state.number != GAMEPAD_AXIS_RIGHT_STICK_X)
            return 0.0f;

        return (float)m_state.value / (float)std::numeric_limits<signed short>::max();
    }

    float Gamepad::RightStickY() {
        if (!IsConnected())
            return 0.0f;
            
        if (m_state.type != JS_EVENT_AXIS && m_state.number != GAMEPAD_AXIS_RIGHT_STICK_Y)
            return 0.0f;

        return (float)m_state.value / (float)std::numeric_limits<signed short>::max();
    }

    float Gamepad::LeftTrigger() {
        if (!IsConnected())
            return 0.0f;
            
        if (m_state.type != JS_EVENT_AXIS && m_state.number != GAMEPAD_AXIS_LEFT_TRIGGER)
            return -1.0f;

        return (float)m_state.value / (float)std::numeric_limits<signed short>::max();
    } 

    float Gamepad::RightTrigger() {
        if (!IsConnected())
            return 0.0f;

        if (m_state.type != JS_EVENT_AXIS && m_state.number != GAMEPAD_AXIS_RIGHT_TRIGGER)
            return -1.0f;

        return (float)m_state.value / (float)std::numeric_limits<signed short>::max();
    }

    // In the linux joystick api xbox dpads are considered as xes
    // but in this api its considered as buttons for consistency with XInput (Windows)
    bool HandleDpad(GamepadButton button, const NativeGamepadState& state) {
        if (state.type != JS_EVENT_AXIS)
            return false;

        if (button == GamepadButton::DpadUp && state.number == GAMEPAD_AXIS_DPAD_Y) 
            return state.value < 0;
        else if (button == GamepadButton::DpadDown && state.number == GAMEPAD_AXIS_DPAD_Y) 
            return state.value > 0;
        else if (button == GamepadButton::DpadRight && state.number == GAMEPAD_AXIS_DPAD_X) 
            return state.value > 0;
        else if (button == GamepadButton::DpadLeft && state.number == GAMEPAD_AXIS_DPAD_X)
            return state.value < 0;

        return false;
    }

    bool Gamepad::IsButtonDown(GamepadButton button) {
        if (!IsConnected())
            return false;

        if (button == GamepadButton::DpadDown || button == GamepadButton::DpadUp || button == GamepadButton::DpadLeft || button == GamepadButton::DpadRight)
            return HandleDpad(button, m_state);

        if (m_state.type != JS_EVENT_BUTTON)
            return false;

        if (m_state.number == (signed short)button)
            return (bool)m_state.value;

        return false;
    }

    bool Gamepad::IsButtonUp(GamepadButton button) { return !IsButtonDown(button); }

    void Gamepad::Update() {
        if (read(m_js, &m_state, sizeof(js_event)) != sizeof(js_event)) {
            if (m_connectedGamepads[m_gamepadIndex])
                m_connectedCallback((GamepadID)m_gamepadIndex, false);
            m_connectedGamepads[m_gamepadIndex] = false;
            return;
        }

        if (!m_connectedGamepads[m_gamepadIndex])
            m_connectedCallback((GamepadID)m_gamepadIndex, true);

        m_connectedGamepads[m_gamepadIndex] = true;

        // switch (m_state.type) {
        //     case JS_EVENT_BUTTON: {
        //         const char* inputState = m_state.value ? "pressed\n" : "released\n";
        //         std::cout << "Button: " << (int)m_state.number << " was " << inputState;
        //         break;
        //     }
        //     case JS_EVENT_AXIS: {
        //         std::cout << "Axis " << (int)m_state.number << " Value: " << m_state.value << "\n";

        //         break;
        //     }
        //     default:
        //         break;
        // }
    }

    bool Gamepad::IsConnected() { return m_connectedGamepads[m_gamepadIndex]; }

    NativeGamepadState Gamepad::GetState() { return m_state; }

    GamepadID Gamepad::GetID() { return (GamepadID)m_gamepadIndex; }

    void Gamepad::SetConnectedCallback(GamepadConnectedCallback callback) { m_connectedCallback = callback; }

    void Gamepad::SetUserPointer(GamepadID gid, void* ptr) { m_userPtrs[(int64_t)gid] = ptr; }
    void* Gamepad::GetUserPointer(GamepadID gid) { m_userPtrs[(int64_t)gid]; }


    void Gamepad::Rumble(float leftMotor = 0.0f, float rightMotor = 0.0f) {
        // Windows only for now
    }

}