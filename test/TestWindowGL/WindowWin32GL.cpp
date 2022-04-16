#include "IWindow.h"
#include "IWindowGL.h"

#include <gl/gl.h>

#include <iostream>

constexpr uint16_t GL_VERSION_MAJOR = 3;
constexpr uint16_t GL_VERSION_MINOR = 3;

constexpr int64_t WINDOW_WIDTH = 1280;
constexpr int64_t WINDOW_HEIGHT = 720;

int main() {
    IWindow::Window window{};
    IWindow::GL::Context glcontext{};

    if (!window.Create(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello IWindow")) return EXIT_FAILURE;
    if (!glcontext.Create(window, GL_VERSION_MAJOR, GL_VERSION_MINOR)) return EXIT_FAILURE;
    
    if (GL_VERSION_MAJOR > 2) {
        auto glGenBuffers =
            (void (*)(GLsizei, GLuint*))IWindow::GL::LoadOpenGLFunction("glGenBuffers");

        uint32_t bufferExample = 0;
        glGenBuffers(1, &bufferExample);

        std::cout << "Buffer Value: " << bufferExample << '\n';
    }

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


        if (window.IsKeyDown(IWindow::Key::A, IWindow::Key::W) && window.IsKeyUp(IWindow::Key::Alt))
            std::cout << "Key A and W were pressed without Alt being pressed\n";

        window.Update();
        glcontext.SwapBuffers();
    }
}