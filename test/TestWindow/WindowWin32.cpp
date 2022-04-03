#include "IWindow.h"

#include <iostream>

void WindowPosCallback(IWindow::Window& window, uint32_t x, uint32_t y) {
    std::cout << x << ", " << y << '\n';
}

int main() {
    IWindow::Window window = IWindow::Window(1280, 720, "Hello IWindow");

    window.SetPosCallback(WindowPosCallback);

    
    while (window.IsRunning()) {

        window.Update();
    }
}