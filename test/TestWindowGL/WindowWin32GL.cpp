#include "IWindow.h"
#include "IWindowGL.h"

#include <glad/glad.h>

#include <iostream>

constexpr uint16_t GL_VERSION_MAJOR = 3;
constexpr uint16_t GL_VERSION_MINOR = 3;

constexpr int64_t WINDOW_WIDTH = 1280;
constexpr int64_t WINDOW_HEIGHT = 720;



uint32_t GLCreateVao() { 
    uint32_t vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

uint32_t GLCreateBuffer(int bufferType, void* data, size_t dataSize) {
    uint32_t buffer = 0;
    glGenBuffers(1, &buffer);
    glBindBuffer(bufferType, buffer);

    glBufferData(bufferType, dataSize, data, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (const void*)0);
    glEnableVertexAttribArray(0);
}

uint32_t GLCreateShader(int shaderType) {
    uint32_t shader = 0;

    const char* vertexShaderSource = R"(#version 330 core
    layout (location = 0) in vec3 aPos;
    void main()
    {
       gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    })";

    const char* fragmentShaderSource = R"(#version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    } 
    )";
        glShaderSource(shader, 1, (const char* const*)"", nullptr);
}
uint32_t GLCreateShaderProgram() {

}

int main() {
    IWindow::Window window{};
    IWindow::GL::Context glcontext{};

    if (!window.Create(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello IWindow")) return EXIT_FAILURE;

    if (!glcontext.Create(window, GL_VERSION_MAJOR, GL_VERSION_MINOR)) { 
        MessageBoxA(nullptr, "Failed to create a IWindow context!\nThis is probably because your computor doesn't support the required version of OpenGL", "Error", MB_ICONEXCLAMATION | MB_OK);
        return EXIT_FAILURE;
    }
    
    if (!gladLoadGL()) {
        MessageBoxA(nullptr, "Failed to load glad!", "Error", MB_ICONEXCLAMATION | MB_OK);
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
    }
}