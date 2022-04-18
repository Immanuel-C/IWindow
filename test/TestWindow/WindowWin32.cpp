#include "IWindow.h"
#include "IWindowGamepad.h"

#include <iostream>
#include <chrono>
#include <future>

namespace Example {
    void WindowPosCallback(IWindow::Window& window, int64_t x, int64_t y) {
        // std::cout << "Window position: " << x << ", " << y << '\n';
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

void MouseButtonCallback(IWindow::Window& window, IWindow::MouseButton button, IWindow::InputState iState, IWindow::ClickState cState) {
    const char* sButton = "";
    const char* sIState = "";
    const char* sCState = "";

    using namespace IWindow;

    switch (button) {
    case MouseButton::Left:
        sButton = "Left";
        break;
    case MouseButton::Right:
        sButton = "Right";
        break;
    case MouseButton::Middle:
        sButton = "Middle";
        break;
    case MouseButton::Side1:
        sButton = "Side1";
        break;
    case MouseButton::Side2:
        sButton = "Side2";
        break;
    }


    switch (iState)
    {
    case IWindow::InputState::Down:
        sIState = "pressed";
        break;
    case IWindow::InputState::Up:
        sIState = "released";
        break;
    default:
        break;
    }

    switch (cState)
    {
    case IWindow::ClickState::Double:
        sCState = "2";
        break;
    case IWindow::ClickState::Single:
        sCState = "1";
        break;
    case IWindow::ClickState::Up:
        sCState = "up";
        break;
    default:
        break;
    }


    std::cout << "Mouse button " << sButton << " was clicked times " << sCState << " and was " << sIState << '\n';
}

void MouseMoveCallback(IWindow::Window& window, int64_t x, int64_t y) {
    // std::cout << "Mouse Moved: " << x << ", " << y << '\n';
}



int main() {
    IWindow::Window window{};
    if (!window.Create(1280, 720, "Hello IWindow")) return EXIT_FAILURE;

    // 0 is the index/ID of the gamepad
    IWindow::Gamepad gp{ IWindow::GamepadID::GP1 };

    int windowUserPtrExample = 10;
    uint32_t gamePadsConnected = 0;

    window.SetUserPointer(&windowUserPtrExample);
    window.SetPosCallback(Example::WindowPosCallback);
    window.SetSizeCallback(WindowSizeCallback);
    window.SetKeyCallback(KeyCallback);
    window.SetMouseButtonCallback(MouseButtonCallback);
    window.SetMouseMoveCallback(MouseMoveCallback);

    for (uint32_t i = 0; i < (int)IWindow::GamepadID::Max; i++) 
        IWindow::Gamepad::SetUserPointer((IWindow::GamepadID)i, &gamePadsConnected);

    IWindow::Gamepad::SetConnectedCallback(GamepadConnectedCallback);

    window.Fullscreen(true);

    while (window.IsRunning()) {

        gp.Rumble();

        if (window.IsKeyDown(IWindow::Key::A, IWindow::Key::W) && window.IsKeyUp(IWindow::Key::Alt))
            std::cout << "Key A and W were pressed without Alt being pressed\n";

        if (gp.IsButtonDown(IWindow::GamepadButton::A)) {
            std::cout << "A was pressed!\n";
            gp.Rumble(0.25, 0.35);
        }

        if (window.IsMouseButtonDoubleClicked(IWindow::MouseButton::Left)) {
            std::cout << "Double clicked the left mouse button!\n";
        }

        // std::cout << window.GetMousePosition().x << ", " << window.GetMousePosition().y << '\n';

        // std::cout << "Left Stick X: " << gp.RightTrigger() << '\n';

        //std::cout << "Right Trigger: " << gp.RightTrigger() << '\n';


        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        window.Fullscreen(false);

        gp.Update();
        window.Update();
    }
}