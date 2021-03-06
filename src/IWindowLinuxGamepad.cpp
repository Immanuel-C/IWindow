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
#if !defined(_WIN32)
#include "IWindowGamepad.h"

#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <linux/input.h>
#include <iostream>
#include <limits>
#include <dirent.h>
#include <string.h>


namespace IWindow {
    std::array<bool, (int)GamepadID::Max> Gamepad::m_connectedGamepads{ false };
    std::array<void*, (int)GamepadID::Max> Gamepad::m_userPtrs{ nullptr };
    GamepadConnectedCallback Gamepad::m_connectedCallback = Gamepad::DefaultGamepadConnectedCallback;
    std::array<bool, (int)GamepadID::Max> hasSentEffect{ false };

    static constexpr int32_t GAMEPAD_AXIS_LEFT_STICK_Y = 0;
    static constexpr int32_t GAMEPAD_AXIS_LEFT_STICK_X = 1;
    static constexpr int32_t GAMEPAD_AXIS_RIGHT_STICK_Y = 3;
    static constexpr int32_t GAMEPAD_AXIS_RIGHT_STICK_X = 4;
    
    static constexpr int32_t GAMEPAD_AXIS_LEFT_TRIGGER = 2;
    static constexpr int32_t GAMEPAD_AXIS_RIGHT_TRIGGER = 5;

    static constexpr int32_t GAMEPAD_AXIS_DPAD_X = 6;
    static constexpr int32_t GAMEPAD_AXIS_DPAD_Y = 7;

    static constexpr int64_t FD_OPEN_MASK = (O_RDONLY | O_NONBLOCK);
    static constexpr int64_t EVENT_FD_OPEN_MASK = O_RDWR;

    // Same as XInput XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
    static constexpr int16_t GAMEPAD_LEFT_STICK_DEADZONE = 7849;
    // Same as XInput XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
    static constexpr int16_t GAMEPAD_RIGHT_STICK_DEADZONE = 8689;

    /* TEST_BIT  : Courtesy of Johan Deneux */
    #define BITS_PER_LONG (sizeof(long) * 8)
    #define OFF(x)  ((x)%BITS_PER_LONG)
    #define BIT(x)  (1UL<<OFF(x))
    #define LONG(x) ((x)/BITS_PER_LONG)
    #define TEST_BIT(bit, array)    ((array[LONG(bit)] >> OFF(bit)) & 1)



    bool DirExists(const std::string& filename) {
        DIR* dir;
        dir = opendir(filename.c_str());
        
        if (!dir)
            return false;
        
        closedir(dir);
        return true;
    }

    Gamepad::Gamepad(GamepadID gamepadIndex) : m_gamepadIndex { (int)gamepadIndex }{
        m_devPath = "/dev/input/js" + std::to_string(m_gamepadIndex);
        m_js = open(m_devPath.c_str(), FD_OPEN_MASK);
        // -1 if failed
        // if (m_js == -1)  std::cout << "Failed to open: \"" << m_devPath << "\" for gamepad input!\n";
        if (m_js == -1) {
            m_connectedGamepads[m_gamepadIndex] = false;
            return;
        }

        m_connectedGamepads[m_gamepadIndex] = true;
        m_connectedCallback(gamepadIndex, true);

        for (uint32_t i = 0; i < 99; i++) {
            using namespace std::string_literals;
            std::string tmpEventFileName = "/sys/class/input/js" + std::to_string(m_gamepadIndex) + "/device/event" + std::to_string(i);
            if (DirExists(tmpEventFileName)) {
                m_devEventPath = "/dev/input/event" + std::to_string(i);
                m_event = open(m_devEventPath.c_str(), EVENT_FD_OPEN_MASK);
                if (m_event != -1) {
                    std::array<unsigned long, 4> features;
                    // Ask for device features. If it failes rumble is not supported
                    if (ioctl(m_event, EVIOCGBIT(EV_FF, sizeof(unsigned long) * 4), features.data()) != -1) {
                        if (TEST_BIT(FF_RUMBLE, features))
                            break;
                    }
                }
            }
        }

        if (m_event == -1) return;

        ff_effect effect{};

        effect.type = FF_RUMBLE;
        effect.u.rumble.strong_magnitude = UINT16_MAX / 2;
        effect.u.rumble.weak_magnitude = UINT16_MAX / 4;
        // One second converted to miliseconds
        effect.replay.length = 1 * 1000;
        effect.replay.delay = 0;
        effect.id = -1;

        // Send the effect to the driver
        if (ioctl(m_event, EVIOCSFF, &effect) == -1) {
            std::cerr << "Failed to send gamepad effect to the driver! Error string: " << strerror(errno) << '\n';
            return;
        }


    } 

    Gamepad::~Gamepad() {            
        if (m_js != -1) close(m_js);
        if (m_event != -1) close(m_event); 
    }

    bool Gamepad::IsLeftStickInDeadzone() {
        if (!IsConnected())


        if (m_state.type != JS_EVENT_AXIS && m_state.number != GAMEPAD_AXIS_LEFT_STICK_Y || m_state.number != GAMEPAD_AXIS_LEFT_STICK_X)
            return false;

        if (m_state.value > GAMEPAD_LEFT_STICK_DEADZONE || m_state.value < -GAMEPAD_LEFT_STICK_DEADZONE && m_state.number == GAMEPAD_AXIS_LEFT_STICK_X)
            return false;

        if (m_state.value > GAMEPAD_LEFT_STICK_DEADZONE || m_state.value < -GAMEPAD_LEFT_STICK_DEADZONE && m_state.number == GAMEPAD_AXIS_LEFT_STICK_Y)
            return false;

        return true; 
    }

    bool Gamepad::IsRightStickInDeadzone() {
        if (!IsConnected())
            return false;
            
        if (m_state.type != JS_EVENT_AXIS && m_state.number != GAMEPAD_AXIS_RIGHT_STICK_X || m_state.number != GAMEPAD_AXIS_RIGHT_STICK_Y)
            return false;

        if (m_state.value > GAMEPAD_RIGHT_STICK_DEADZONE || m_state.value < -GAMEPAD_RIGHT_STICK_DEADZONE && m_state.number == GAMEPAD_AXIS_RIGHT_STICK_X)
            return false;

        if (m_state.value > GAMEPAD_RIGHT_STICK_DEADZONE || m_state.value < -GAMEPAD_RIGHT_STICK_DEADZONE && m_state.number == GAMEPAD_AXIS_RIGHT_STICK_Y)
            return false;

        return true; 
    }


    float Gamepad::LeftStickX() {
        if (!IsConnected())
            return 0.0f;
            
        if (m_state.type != JS_EVENT_AXIS && m_state.number != GAMEPAD_AXIS_LEFT_STICK_X)
            return 0.0f;

        // The range is usally between std::numeric_limits<signed short>::max() and std::numeric_limits<signed short>::min()
        // but we want it to be in range -1.0f and 1.0f for consistency 
        return (float)m_state.value / (float)std::numeric_limits<signed short>::max();
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
        // if read != sizeof(NativeGamepadState) then it failed
        // if read == -1 it means that the joystick file descriptor sent no event 
        // It will only return this if the O_NONBLOCK flag is set when opening the file descriptor

        int readOp = read(m_js, &m_state, sizeof(NativeGamepadState)); 

        // If read failed its probably because the js fd has failed to open because no gamepads are detected
        if (readOp != sizeof(NativeGamepadState) && m_js == -1) {
            if (m_connectedGamepads[m_gamepadIndex]) {
                m_connectedCallback((GamepadID)m_gamepadIndex, false);
                m_connectedGamepads[m_gamepadIndex] = false;
            }
            m_js = open(m_devPath.c_str(), FD_OPEN_MASK);
            return;
        }

        if (!m_connectedGamepads[m_gamepadIndex]) {
            m_connectedCallback((GamepadID)m_gamepadIndex, true);
            m_connectedGamepads[m_gamepadIndex] = true;
        }
    }

    bool Gamepad::IsConnected() { return m_connectedGamepads[m_gamepadIndex]; }

    NativeGamepadState Gamepad::GetState() { return m_state; }

    GamepadID Gamepad::GetID() { return (GamepadID)m_gamepadIndex; }

    void Gamepad::SetConnectedCallback(GamepadConnectedCallback callback) { m_connectedCallback = callback; }

    void Gamepad::SetUserPointer(GamepadID gid, void* ptr) { m_userPtrs[(int64_t)gid] = ptr; }
    void* Gamepad::GetUserPointer(GamepadID gid) { return m_userPtrs[(int64_t)gid]; }

    // No mutex would be required for this function
    void RumbleAsync(int event, float leftMotor, float rightMotor) {

        input_event play{}, stop{};

        play.type = EV_FF;
        play.code = -1;
        // 1 = play rumble effect once
        play.value = 1;


        stop.type = EV_FF;
        stop.code = -1;
        // 0 = stop rumble effect 
        stop.value = 0;


        if (leftMotor <= 0.0f || rightMotor <= 0.0f) 
            return;

        // Write stop event
        if (write(event, &stop, sizeof(input_event)) == -1) { 
            std::cerr << "Failed to write rumble stop event to driver! Error string: " << strerror(errno) << '\n';
            return; 
        }

        // Write play event
        if (write(event, &play, sizeof(input_event)) == -1) { 
            std::cerr << "Failed to write rumble play event to driver! Error string: " << strerror(errno) << '\n';
            return; 
        }

        std::cerr << "Errno: " << strerror(errno) << '\n';



        std::this_thread::sleep_for(std::chrono::seconds{1});
    }

    // TODO(Immu): Since the linux api doesn't support setting each motor seperatly the function should change to only one value passed in
    void Gamepad::Rumble(float leftMotor, float rightMotor) {
        if (!IsConnected() || m_event == -1)
            return;

        // If the future is not valid we cant call std::future::wait_for
        if (!m_rumbleFuture.valid()) {
            m_rumbleFuture = std::async(std::launch::async, RumbleAsync, m_event, leftMotor, rightMotor);
            return;
        }


        // if the future is not ready and we try to reset it, it will wait until the previous thread is done which would pause the main thread.
        if (m_rumbleFuture.wait_for(std::chrono::seconds{0}) == std::future_status::ready)
            m_rumbleFuture = std::async(std::launch::async, RumbleAsync, m_event, leftMotor, rightMotor);

        // ff_effect effect{};

        // effect.type = FF_RUMBLE;
        // effect.u.rumble.strong_magnitude = 0x8000;
        // effect.u.rumble.weak_magnitude = 0;
        // effect.replay.length = 1000;
        // effect.replay.delay = 0;

        // effect.id = -1;


        // input_event play{}, stop{};

        // play.type = EV_FF;
        // play.code = effect.id;
        // // 1 = play rumble effect once
        // play.value = 1;


        // stop.type = EV_FF;
        // stop.code = effect.id;
        // // 0 = stop rumble effect 
        // stop.value = 0;


        // if (leftMotor <= 0.0f || rightMotor <= 0.0f) 
        //     return;


        // // Send the effect to the driver
        // if (ioctl(m_event, EVIOCSFF, &effect) == -1) {
        //     std::cerr << "Failed to send gamepad effect to the driver! Error string: " << strerror(errno) << '\n';
        //     return;
        // }

        // // Write play event
        // if (write(m_event, &play, sizeof(input_event)) == -1) { 
        //     std::cerr << "Failed to write rumble play event to driver! Error string: " << strerror(errno) << '\n';
        //     return; 
        // }

        // // Stop any previous rumble effect and deletes any effects in the gamepads memory
        // if (ioctl(m_event, EVIOCRMFF, effect.id) == -1) {
        //    std::cerr << "Failed to delete gamepad effect on gamepad! Error string: " << strerror(errno) << '\n';
        //    return;
        // }
    }

}
#endif // _WIN32