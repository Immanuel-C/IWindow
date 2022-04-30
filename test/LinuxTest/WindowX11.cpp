#include "IWindow.h"
#include "IWindowGL.h"
#include "IWindowGamepad.h"
#include <iostream>

int main()
{
    IWindow::Window window{};
    IWindow::GL::Context glContext{};
    IWindow::Gamepad gp{ IWindow::GamepadID::GP1 };

    if (!window.Create(1280, 720, u8"Hello IWindow!")) {
        std::cout << "failed to create an IWindow window! Abort!\n";
        return EXIT_FAILURE;
    }

    if (!glContext.Create(window, 3, 1)) {
        std::cerr << "Failed to create an OpenGL Context!\n";
        return EXIT_FAILURE;
    }

    window.SetWindowSize(1280, 720);

    //window.Fullscreen(true, window.GetPrimaryMonitor());
    window.Fullscreen(false, window.GetPrimaryMonitor());


    IWindow::Monitor primaryMonitor = window.GetPrimaryMonitor();
    std::vector<IWindow::Monitor> monitors = window.GetAllMonitors();

    for (IWindow::Monitor& monitor : monitors) {
        // IWindow::Monitor::name is a std::wstring
        // std::wcout << "Monitor Name: " << monitor.name << "\nMonitor Size: " << monitor.size.x << ", " << monitor.size.y << '\n';
    }

    while (window.IsRunning())
    {
        glClear(GL_COLOR_BUFFER_BIT);


        glBegin(GL_POLYGON);
            glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 0.5);
            glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 0);
            glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
        glEnd();


        //std::cout << "Scroll Offset: " << window.GetMouseScrollOffset().x << ", " << window.GetMouseScrollOffset().y << '\n';
        
        // std::cout << "Left Stick X: " << gp.LeftStickX() << '\n';

        if (gp.IsButtonDown(IWindow::GamepadButton::A))
            std::cout << "A button is down!\n";
        if (gp.IsButtonDown(IWindow::GamepadButton::DpadRight))
            std::cout << "Dpad right is down!\n";

        if (window.IsMouseButtonDown(IWindow::MouseButton::Side2))
            std::cout << "Mouse Button Down!\n";

        if (window.IsKeyDown(IWindow::Key::A))
            std::cout << "A key is down!\n";

        window.Update(); 
        glContext.SwapBuffers();
        gp.Update();
    }

    return EXIT_SUCCESS;
}