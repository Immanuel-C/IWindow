#include <glad/glad.h>
#include "IWindow.h"
#include "IWindowGL.h"
#include "IWindowGamepad.h"



#include <iostream>

constexpr uint16_t GL_VERSION_MAJOR = 3;
constexpr uint16_t GL_VERSION_MINOR = 3;

constexpr int64_t WINDOW_WIDTH = 1280;
constexpr int64_t WINDOW_HEIGHT = 720;

int main() {
    IWindow::Window window{};
    IWindow::GL::Context glcontext{};
    IWindow::Gamepad gp{ IWindow::GamepadID::GP1 };

    if (!window.Create(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello IWindow")) return EXIT_FAILURE;

    if (!glcontext.Create(window, GL_VERSION_MAJOR, GL_VERSION_MINOR)) { 
        std::cout << "Failed to create a IWindow context!\nThis is probably because your computor doesn't support the required version of OpenGL\n";
        return EXIT_FAILURE;
    }
    
    if (!gladLoadGL()) {
        std::cout << "Failed to load glad!\n";
        return EXIT_FAILURE;
    }

    if (GL_VERSION_MAJOR > 2)
        auto myglGenBuffers = (void (*)(GLsizei, GLuint*))IWindow::GL::LoadOpenGLFunction("glGenBuffers");


    while (window.IsRunning()) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

        if (GL_VERSION_MAJOR < 3) {
            glBegin(GL_POLYGON);
                glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 0.5);
                glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 0);
                glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
            glEnd();
        }

        window.Update();
        glcontext.SwapBuffers();
        gp.Update();
    }
}