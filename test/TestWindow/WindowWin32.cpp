#include "IWindow.h"
#include "IWindowGamepad.h"

#include <iostream>
#include <chrono>
#include <future>

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


    std::cout << "Key: " << (int)key << "is being/was just " << sState << '\n';
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
    default:
        break;
    }

    if (gamePadsConnected)
        std::cout << "Gamepads connected: " << *gamePadsConnected << '\n';

    std::cout << "Gamepad " << (int)gid << " was " << sConnected << "!\n";
}

int main() {
    IWindow::Window window{};
    if (!window.Create(1280, 720, "Hello IWindow")) return EXIT_FAILURE;

    // 0 is the index/ID of the gamepad
    IWindow::Gamepad gp{ 0 };

    int windowUserPtrExample = 10;
    uint32_t gamePadsConnected = 0;

    window.SetPosCallback(Example::WindowPosCallback);
    window.SetSizeCallback(WindowSizeCallback);
    window.SetKeyCallback(KeyCallback);
    window.SetUserPointer(&windowUserPtrExample);

    for (uint32_t i = 0; i < (int)IWindow::GamepadID::Max; i++) 
        IWindow::Gamepad::SetUserPointer((IWindow::GamepadID)i, &gamePadsConnected);

    IWindow::Gamepad::SetGamepadConnectedCallback(GamepadConnectedCallback);
    

    while (window.IsRunning()) {

        gp.Rumble();

        if (window.IsKeyDown(IWindow::Key::A, IWindow::Key::W) && window.IsKeyUp(IWindow::Key::Alt))
            std::cout << "Key A and W were pressed without Alt being pressed\n";

        if (gp.IsButtonDown(IWindow::GamepadButton::A)) {
            std::cout << "A was pressed!\n";
            gp.Rumble(0.25, 0.35);
        }

        //std::cout << "Right Trigger: " << gp.RightTrigger() << '\n';

        gp.Update();
        window.Update();
    }
}