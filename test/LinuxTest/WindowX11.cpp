#include "IWindow.h"
#include <iostream>
#include <thread>

int main()
{
    IWindow::Window window{};

    if (!window.Create(1280, 720, u8"Hello IWindow!"))
    {
        std::cout << "failed to create an IWindow window! Abort!\n";
        return EXIT_FAILURE;
    }

    window.SetWindowSize(1280, 720);

    //window.Fullscreen(true, window.GetPrimaryMonitor());
    window.Fullscreen(false, window.GetPrimaryMonitor());


    IWindow::Monitor primaryMonitor = window.GetPrimaryMonitor();
    std::vector<IWindow::Monitor> monitors = window.GetAllMonitors();

    for (IWindow::Monitor& monitor : monitors) {
        // IWindow::Monitor::name is a std::wstring
        //std::wcout << "Monitor Name: " << monitor.name << "\nMonitor Size: " << monitor.size.x << ", " << monitor.size.y << '\n';
    }

    while (window.IsRunning())
    {
        std::cout << "Scroll Offset: " << window.GetMouseScrollOffset().x << ", " << window.GetMouseScrollOffset().y << '\n';
        
        if (window.IsMouseButtonDown(IWindow::MouseButton::Side2))
            std::cout << "Mouse Button Down!\n";

        if (window.IsKeyDown(IWindow::Key::A))
            std::cout << "A key is down!\n";
        window.Update(); 
    }

    return EXIT_SUCCESS;
}