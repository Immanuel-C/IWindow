#include "IWindow.h"

#include <iostream>

void WindowPosCallback(IWindow::Window& window, uint32_t x, uint32_t y) {
    //std::cout << x << ", " << y << '\n';
}


void WindowSizeCallback(IWindow::Window& window, uint32_t width, uint32_t height) {
    std::cout << "Resized!\n";
}

int main() {
    IWindow::Window window = IWindow::Window(1280, 720, "Hello IWindow");

    window.SetPosCallback(WindowPosCallback);
    window.SetSizeCallback(WindowSizeCallback);
    
    while (window.IsRunning()) {

        window.Update();
    }
}