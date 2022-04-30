# Gamepad Input In IWindow

Gamepad input is pretty simple in IWindow. 

There is a seperate class for gamepad input instead of being in the window class.

IWindow can only have upto 4 gamepads connected to the whole application.

The class is `IWindow::Gamepad`.

```cpp
namespace IWindow {
    class Gamepad {
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
    };
}

```

## Main Functions

`IWindow::Gamepad::Gamepad(IWindow::GamepadID gamepadIndex)` takes in 1 argument, `IWindow::GamepadID gamepadIndex`. Game index is one of the 4 available controller slots.

`void IWindow::Gamepad::Update()` updates the gamepad state. call this every frame.

`bool IWindow::Gamepad::IsLeftStickInDeadzone()` and `bool IsRightStickInDeadzone()` checks if the controller is in the dead zone of the left/right stick.

`float IWindow::Gamepad::LeftStickX()` and `float IWindow::Gamepad::RightStickX()` gets the value of how far is the sticks x axis is to the left (-1) or right (0) on the left/right stick.

`float IWindow::Gamepad::LeftStickY()` and `float IWindow::Gamepad::RightStickY()` gets the value of how far is the sticks y axis is to the bottom (-1) or top (1) on the left/right stick.

`float IWindow::Gamepad::LeftTrigger()` and `float IWindow::Gamepad::RightTrigger()` gets the value of how much the left/right trigger is down. The functions return 0 if the left/right trigger completly up or it will return 1 if the left/right trigger completly down.

`bool IWindow::Gamepad::IsButtonDown(GamepadButton button)` checks if argument `button` is being pressed. An example of a `GamepadButton` is `GamepadButton::A`  and `GamepadButton::B`.

`bool IWindow::Gamepad::IsButtonUp(GamepadButton button)` checks if argument `button` is not being pressed.

`IWindow::GamepadID IWindow::Gamepad::GetID()` gets the id return the ID that was passed to the constructor. 

`bool IWindow::Gamepad::IsConnected()` checks if the a controller with the ID of the value passed into the constructor is connected.

`static void IWindow::Gamepad::SetConnectedCallback(GamepadConnectedCallback callback)` sets the connected callback. Check out [Callbacks](./Callbacks.md) for more info.


`static void IWindow::Gamepad::SetUserPointer(IWindow::GamepadID gid, void* ptr)` sets a pointer that is associated to the GamepadID.
`static void* IWindow::Gamepad::GetUserPointer(IWindow::GamepadID gid)` gets the pointer associated with the GamepadID. nullptr if no pointer is set.

## Win32 Only

If using Xlib this wont do anything.

`void IWindow::Gamepad::Rumble(float leftMotor = 0.0f, float rightMotor = 0.0f)` vibrates the gamepads motors. leftMotor controlles the left motor on the gamepad and the rightMotor controlles the right motor on the gamepad. Setting the a value to 0 will stop vibrations and setting it to 1 will set the motors to the max speed.


## Advanced Functions

`GamepadState IWindow::Gamepad::GetState()` get the native gamepad api's state (e.g. XInput: `XINPUT_STATE`).
