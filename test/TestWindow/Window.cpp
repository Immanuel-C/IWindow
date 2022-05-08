#include "IWindow.h"
#include "IWindowGamepad.h"
#include "stbi.h"

#include <iostream>
#include <unistd.h>


namespace Example {
    void WindowPosCallback(IWindow::Window& window, int64_t x, int64_t y) {
        std::cout << "Window position: " << x << ", " << y << '\n';
    }
}

void WindowSizeCallback(IWindow::Window& window, int64_t width, int64_t height) {
    int* userPtr = (int*)window.GetUserPointer();
    if (userPtr) std::cout << "User Pointer: " << *userPtr << '\n';
    std::cout << "Window Size: " << width << ", " << height << '\n';
}

void KeyCallback(IWindow::Window& window, IWindow::Key key, IWindow::InputState state) {
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

void MouseButtonCallback(IWindow::Window& window, IWindow::MouseButton button, IWindow::InputState state) {
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

void MouseMoveCallback(IWindow::Window& window, int64_t x, int64_t y) {
    std::cout << "Mouse Moved: " << x << ", " << y << '\n';
}

void MouseScrollCallback(IWindow::Window& window, float xOffset, float yOffset) {
    std::cout << "x Offset: " << xOffset << " y Offset: " << yOffset << '\n';
}

int main() {
    IWindow::Window window{};
    if (!window.Create(1280, 720, u8"Hello IWindow!")) return EXIT_FAILURE;
    // window.SetCursor(IWindow::NativeCursorID::Hand); // Using internal windowing apis cursors
    // window.SetIcon(IWindow::NativeIconID::Default); // Using internal windowing apis icons

    int width = 0, height = 0, ch = 0;

    uint8_t* data = stbi_load("assets/icon.png", &width, &height, &ch, 4);

    IWindow::Image image{};
    image.width = width;
    image.height = height;
    image.data = data;

    window.SetIcon(image);
    // 0, 0 is top left of cursor
    // width, height is bottom right of cursor
    // width / 2, height / 2 is middle of cursor
    window.SetCursor(image, 0, 0);

    stbi_image_free(data); // delete[] data;


    IWindow::Gamepad gp{ IWindow::GamepadID::GP2 };

    int windowUserPtrExample = 10;
    uint32_t gamePadsConnected = 0;

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


    // Set fullscreen on the primary monitor then wait 1 second then set to no fullscreen
    // on the primary monitor
    // window.Fullscreen(true, window.GetPrimaryMonitor());
    // sleep(1);
    // window.Fullscreen(false, window.GetPrimaryMonitor());

    // Gets all available monitors
    std::vector<IWindow::Monitor> monitors = window.GetAllMonitors();


    /*
    // Print monitor properties
    for (IWindow::Monitor& monitor : monitors) 
        // monitor.name is a std::wstring
        std::wcout << "Monitor Name: " << monitor.name << '\n'
            << "Monitor Size: " << monitor.size.x << ", " << monitor.size.y << '\n'
            << "Monitor Position: " << monitor.position.x << ", " << monitor.position.y << '\n';
    */  


    while (window.IsRunning()) {
        //gp.Rumble();

        if (window.IsKeyDown(IWindow::Key::A, IWindow::Key::W) && window.IsKeyUp(IWindow::Key::Alt))
            std::cout << "Key A and W were pressed without Alt being pressed\n";

        if (gp.IsButtonDown(IWindow::GamepadButton::A)) {
            std::cout << "A was pressed!\n";
            gp.Rumble(1.0f, 0.35f);
        }

        gp.Update();
        window.Update();
    }


    return EXIT_SUCCESS;
}