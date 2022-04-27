#include "IWindow.h"
#include <iostream>
#include <stdexcept>
#include <unistd.h>

int main()
{
    IWindow::Window window{};

    if (!window.Create(1280, 720, u8"Hello IWindow!"))
    {
        printf("failed to create an IWindow window! Abort!\n");
        return EXIT_FAILURE;
    }

    window.SetWindowSize(1000, 1000);

    bool oldPress = false;

    while (window.IsRunning())
    {
        
        window.Update();

        if (window.IsKeyDown(IWindow::Key::A))
        {
            if (oldPress == false)
                printf("A key DOWN!\n");
            oldPress = true;
        }
        else
        {
            oldPress = false;
        }
    }

    window.~Window();

    return EXIT_SUCCESS;
}