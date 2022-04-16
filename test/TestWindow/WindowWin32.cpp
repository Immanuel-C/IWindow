#include "IWindow.h"

#include <iostream>

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

int main() {
    IWindow::Window window{};
    if (!window.Create(1280, 720, "Hello IWindow")) return EXIT_FAILURE;

    int userPtrExample = 10;

    window.SetPosCallback(Example::WindowPosCallback);
    window.SetSizeCallback(WindowSizeCallback);
    window.SetKeyCallback(KeyCallback);
    window.SetUserPointer(&userPtrExample);
    
    while (window.IsRunning()) {

        if (window.IsKeyDown(IWindow::Key::A, IWindow::Key::W) && window.IsKeyUp(IWindow::Key::Alt))
            std::cout << "Key A and W were pressed without Alt being pressed\n";

        window.Update();
    }
}