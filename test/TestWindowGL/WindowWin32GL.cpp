#include "IWindow.h"
#include "IWindowGL.h"
#include <gl/gl.h>


#include <iostream>
int main() {
    IWindow::Window window{};
    IWindow::GL::Context glcontext{};

    if (!window.Create(1280, 720, "Hello IWindow")) return EXIT_FAILURE;
    if (!glcontext.Create(window, 2, 0)) return EXIT_FAILURE;

   glClear(GL_COLOR_BUFFER_BIT);
   glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

    while (window.IsRunning()) {

        glBegin(GL_POLYGON);
            glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 0.5);
            glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 0);
            glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
        glEnd();


        if (window.IsKeyDown(IWindow::Key::A, IWindow::Key::W) && window.IsKeyUp(IWindow::Key::Alt))
            std::cout << "Key A and W were pressed without Alt being pressed\n";

        window.Update();
        glcontext.SwapBuffers();
    }
}