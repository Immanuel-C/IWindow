#include "IWindow.h"
#include "IWindowGamepad.h"
#include "stbi.h"

#include <iostream>
#include <chrono>
#include <future>


namespace Example {
    void WindowPosCallback(IWindow::Window& window, int64_t x, int64_t y) {
        //std::cout << "Window position: " << x << ", " << y << '\n';
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
    //std::cout << "Mouse Moved: " << x << ", " << y << '\n';
}

void MouseScrollCallback(IWindow::Window& window, float xOffset, float yOffset) {
    //  std::cout << "x Offset: " << xOffset << " y Offset: " << yOffset << '\n';
}

int main() {
    IWindow::Window window{};
    IWindow::Window w2{};
    if (!window.Create(1280, 720, u8"Hello IWindow!")) return EXIT_FAILURE;
    if (!w2.Create(1280, 720, u8"Test")) return EXIT_FAILURE;
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

    stbi_image_free(data); // || delete data;


    IWindow::Gamepad gp{ IWindow::GamepadID::GP1 };

    int windowUserPtrExample = 10;
    uint32_t gamePadsConnected = 0;

    // Register callbacks
    window.SetUserPointer(&windowUserPtrExample);
    window.SetPosCallback(Example::WindowPosCallback);
    window.SetSizeCallback(WindowSizeCallback);
    window.SetKeyCallback(KeyCallback);
    window.SetMouseButtonCallback(MouseButtonCallback);
    window.SetMouseMoveCallback(MouseMoveCallback);
    window.SetMouseScrollCallback(MouseScrollCallback);

    for (uint32_t i = 0; i < (int)IWindow::GamepadID::Max; i++) 
        IWindow::Gamepad::SetUserPointer((IWindow::GamepadID)i, &gamePadsConnected);

    IWindow::Gamepad::SetConnectedCallback(GamepadConnectedCallback);

    // Set fullscreen on the primary monitor then wait 1 second then set to no fullscreen
    // on the primary monitor
    // window.Fullscreen(true, window.GetPrimaryMonitor());
    // std::this_thread::sleep_for(std::chrono::seconds{ 1 });
    // window.Fullscreen(false, window.GetPrimaryMonitor());

    // Gets all available monitors
    std::vector<IWindow::Monitor> monitors = window.GetAllMonitors();


    // Print monitor properties
    for (IWindow::Monitor& monitor : monitors) 
        // monitor.name is a std::wstring
        std::wcout << "Monitor Name: " << monitor.name << '\n'
            << "Monitor Size: " << monitor.size.x << ", " << monitor.size.y << '\n'
            << "Monitor Position: " << monitor.position.x << ", " << monitor.position.y << '\n';


    while (window.IsRunning()) {
        gp.Rumble();

        if (window.IsKeyDown(IWindow::Key::A, IWindow::Key::W) && window.IsKeyUp(IWindow::Key::Alt))
            std::cout << "Key A and W were pressed without Alt being pressed\n";

        if (gp.IsButtonDown(IWindow::GamepadButton::A)) {
            std::cout << "A was pressed!\n";
            gp.Rumble(0.25f, 0.35f);
        }


        // std::cout << window.GetMousePosition().x << ", " << window.GetMousePosition().y << '\n';

        // std::cout << "Left Stick X: " << gp.RightTrigger() << '\n';

        //std::cout << "Right Trigger: " << gp.RightTrigger() << '\n';

        std::cout << "ScrollX: " << window.GetMouseScrollOffset().x << " ScrollY: " << window.GetMouseScrollOffset().y << '\n';

        gp.Update();
        window.Update();
        w2.Update();
    }
}