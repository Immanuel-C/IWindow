// Include glad or any other function loader before IWindow
#include <glad/glad.h>
#include "IWindow.h"
#include "IWindowGL.h"
#include "IWindowGamepad.h"
#include "IWindowImGUIBackend.h"
#include "backends/imgui_impl_opengl2.h"

#include <iostream>

constexpr uint16_t GL_VERSION_MAJOR = 2;
constexpr uint16_t GL_VERSION_MINOR = 0;

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplIWindow_Init(window);
    ImGui_ImplOpenGL2_Init();

    io.DisplaySize.x = window.GetWindowSize().x;
    io.DisplaySize.y = window.GetWindowSize().y;

    ImGui::StyleColorsClassic();


    if (GL_VERSION_MAJOR > 2 && GL_VERSION_MINOR > 2) {
        auto myglGenBuffers = (void (*)(GLsizei, GLuint*))IWindow::GL::LoadOpenGLFunction("glGenBuffers");
        uint32_t buf;
        glGenBuffers(1, &buf);
    }
    while (window.IsRunning()) {
        if (GL_VERSION_MAJOR < 3) {
            glBegin(GL_POLYGON);
                glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 0.5);
                glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 0);
                glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
            glEnd();
        }

        // ImGui Loop
        ImGui_ImplOpenGL2_NewFrame();
        ImGui::NewFrame();
        ImGui_ImplIWindow_NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        ImGui::EndFrame();

        window.Update();

        glcontext.SwapBuffers();
        gp.Update();
    }

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplIWindow_Shutdown();
    ImGui::DestroyContext();
}