#include "IWindow.h"
#include "IWindowGamepad.h"
#include "stbi.h"

#include <iostream>


namespace Example {
    void WindowPosCallback(IWindow::Window& window, IWindow::Vector2<int32_t> pos) {
        std::cout << "Window position: " << pos.x << ", " << pos.y << '\n';
    }
}

void WindowSizeCallback(IWindow::Window& window, IWindow::Vector2<int32_t> size) {
    int* userPtr = (int*)window.GetUserPointer();
    if (userPtr) std::cout << "User Pointer: " << *userPtr << '\n';
    std::cout << "Window Size: " << size.width << ", " << size.height << '\n';
}

void KeyCallback(IWindow::Window& window, IWindow::Key key, IWindow::KeyModifier mods, IWindow::InputState state, bool repeated) {
    const char* sState = nullptr;
    
    switch (state) {
    case IWindow::InputState::Down:
        sState = "pressed";
        break;
    case IWindow::InputState::Up:
        sState = "released";
        break;
    }


    std::cout << "Key: " << (int)key << " was just " << sState << '\n';
}

void GamepadConnectedCallback(IWindow::GamepadID gid, bool isConnected) {
    const char* sConnected = nullptr;
    int* gamePadsConnected = (int*)IWindow::Gamepad::GetUserPointer(gid);

    switch (isConnected)
    {
    case true:
        sConnected = "connected";
        if (gamePadsConnected)
            (*gamePadsConnected)++;
        break;
    case false:
        sConnected = "disconnected";
        if (gamePadsConnected)
            (*gamePadsConnected)--;
        break;
    }

    if (gamePadsConnected)
        std::cout << "Gamepads connected: " << *gamePadsConnected << '\n';

    std::cout << "Gamepad " << (int)gid << " was " << sConnected << "!\n";
}

void MouseButtonCallback(IWindow::Window& window, IWindow::MouseButton button, IWindow::KeyModifier mods, IWindow::InputState state) {
    const char* sButton = "";
    const char* sState = "";

    using namespace IWindow;

    switch (button) {
    case MouseButton::Left:
        sButton = "left";
        break;
    case MouseButton::Right:
        sButton = "right";
        break;
    case MouseButton::Middle:
        sButton = "middle";
        break;
    case MouseButton::Side1:
        sButton = "side1";
        break;
    case MouseButton::Side2:
        sButton = "side2";
        break;
    }


    switch (state)
    {
    case IWindow::InputState::Down:
        sState = "pressed";
        break;
    case IWindow::InputState::Up:
        sState = "released";
        break;
    default:
        break;
    }


    std::cout << "Mouse button " << sButton << " was " << sState << '\n';
}

void MouseMoveCallback(IWindow::Window& window, IWindow::Vector2<int32_t> pos) {
    std::cout << "Mouse Moved: " << pos.x << ", " << pos.y << '\n';
}

void MouseScrollCallback(IWindow::Window& window, IWindow::Vector2<float> offset) {
    std::cout << "x Offset: " << offset.x << " y Offset: " << offset.y << '\n';
}

void MouseEnteredCallback(IWindow::Window&, bool entered) {
    std::cout << "Mouse entered: " << entered << '\n';
}

void CharCallback(IWindow::Window& window, char32_t c, IWindow::KeyModifier mods) {
    std::cout << "Char pressed: " << c << '\n';
}

void WindowFocusedCallback(IWindow::Window& window, bool focused) {
    std::cout << "Window is focused: " << focused << '\n';
}

int main() {
    IWindow::Initialize(IWindow::V100);
    // or 
    // IWindow::Initialize(IWindow::CurrentVersion);

    IWindow::Window window{};
    if (!window.Create({ 800, 600 }, L"Hello IWindow!", IWindow::Monitor::GetPrimaryMonitor())) return EXIT_FAILURE;
    // window.SetCursor(IWindow::NativeCursorID::Hand); // Using internal windowing apis cursors
    // window.SetIcon(IWindow::NativeIconID::Default); // Using internal windowing apis icons
    IWindow::Window window2{};

    // Gets all available monitors
    std::vector<IWindow::Monitor> monitors = IWindow::Monitor::GetAllMonitors();

    if (!window2.Create({ 400, 300 }, L"Hello Other Window!", monitors[monitors.size() - 1])) return EXIT_FAILURE;

    int width = 0, height = 0, ch = 0;

    uint8_t* data = stbi_load("assets/icon.png", &width, &height, &ch, 4);

    IWindow::Image image{};
    image.size.width = width;
    image.size.height = height;
    image.data = data;

    window.SetIcon(image);
    // 0, 0 is top left of cursor
    // width, height is bottom right of cursor
    // width / 2, height / 2 is middle of cursor
    window.SetCursor(image, { 0, 0 });

    stbi_image_free(data); // delete[] data;

    IWindow::Gamepad gp{ IWindow::GamepadID::GP0 };

    int windowUserPtrExample = 10;
    int gamePadsConnected = 0;

    window.SetUserPointer(&windowUserPtrExample);

    for (uint32_t i = 0; i < (int)IWindow::GamepadID::Max; i++) 
        IWindow::Gamepad::SetUserPointer((IWindow::GamepadID)i, &gamePadsConnected);

    IWindow::Gamepad::SetConnectedCallback(GamepadConnectedCallback);

    // Register callbacks
    window.SetPosCallback(Example::WindowPosCallback);
    window.SetSizeCallback(WindowSizeCallback);
    window.SetKeyCallback(KeyCallback);
    window.SetMouseButtonCallback(MouseButtonCallback);
    window.SetMouseMoveCallback(MouseMoveCallback);
    window.SetMouseScrollCallback(MouseScrollCallback);
    window.SetMouseEnteredCallback(MouseEnteredCallback);
    window.SetCharCallback(CharCallback);
    window.SetWindowFocusCallback(WindowFocusedCallback);



    // Set fullscreen on the primary monitor then wait 1 second then set to no fullscreen
    // on the primary monitor
    // window.Fullscreen(true, window.GetPrimaryMonitor());
    // sleep(1);
    // window.Fullscreen(false, window.GetPrimaryMonitor());

    // Print monitor properties
    for (IWindow::Monitor& monitor : monitors) 
        // monitor.name is a std::wstring
        std::cout << "Monitor Name: " << monitor.name << '\n'
            << "Monitor Size: " << monitor.size.x << ", " << monitor.size.y << '\n'
            << "Monitor Position: " << monitor.position.x << ", " << monitor.position.y << '\n';

    window.SetClipboardText("Hello World");

    while (window.IsRunning() && window2.IsRunning()) {
        //gp.Rumble();

        if (window.IsKeyDown(IWindow::Key::A, IWindow::KeyModifier::Shift))
            std::cout << "Shift + A was pressed\n";

        if (window.IsKeyDown(IWindow::Key::V, IWindow::KeyModifier::Control)) {
         
            std::string clipboardText = window.GetClipboardText();
            std::cout << "Text in clipboard: " << clipboardText.c_str() << '\n';
        }

        if (gp.IsButtonDown(IWindow::GamepadButton::A, IWindow::GamepadButton::B)) {
            std::cout << "A and B was pressed!\n";
            gp.Rumble(1.0f);
        }
        if (gp.RightTrigger() > gp.GetTriggerDeadzone()) {
            std::cout << "Right Trigger: " << gp.RightTrigger() << std::endl;
        }
        if (gp.LeftTrigger() > gp.GetTriggerDeadzone()) {
            std::cout << "Left Trigger: " << gp.LeftTrigger() << std::endl;
        }
        if (gp.LeftStickX() > gp.GetStickDeadzone() || gp.LeftStickX() < -gp.GetStickDeadzone() || gp.LeftStickY() > gp.GetStickDeadzone() || gp.LeftStickY() < -gp.GetStickDeadzone() ) {
            std::cout << "Left Stick X: " << gp.LeftStickX() << "Left Stick Y: " << gp.LeftStickY() << std::endl;
        }

        if (gp.RightStickX() > gp.GetStickDeadzone() || gp.RightStickX() < -gp.GetStickDeadzone() || gp.RightStickY() > gp.GetStickDeadzone() || gp.RightStickY() < -gp.GetStickDeadzone()) {
            std::cout << "Right Stick X: " << gp.RightStickX() << "Right Stick Y: " << gp.RightStickY() << std::endl;
        }

        gp.Update();
        window2.Update();
        window.Update();
    }

    window.Destroy();
    IWindow::Shutdown();

    return EXIT_SUCCESS;
}