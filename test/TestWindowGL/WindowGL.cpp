// Include glad or any other function loader before IWindow
#include <glad/glad.h>
#include "IWindow.h"
#include "IWindowGL.h"
#include "IWindowGamepad.h"
#include "IWindowImGUIBackend.h"
#include "backends/imgui_impl_opengl3.h"


#include <iostream>

constexpr uint16_t GL_VERSION_MAJOR = 3;
constexpr uint16_t GL_VERSION_MINOR = 3;

constexpr int64_t WINDOW_WIDTH = 1280;
constexpr int64_t WINDOW_HEIGHT = 720;


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

void MouseButtonCallback(IWindow::Window& window, IWindow::MouseButton button, IWindow::InputState state) {
    std::cout << "Mouse button " << (int)button << " was " << (int)state << '\n';
}


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

    glcontext.MakeContextCurrent();
    glcontext.vSync(true);

    window.SetMouseButtonCallback(MouseButtonCallback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplOpenGL3_Init();
    ImGui_ImplIWindow_Init(window);

    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.ConfigWindowsResizeFromEdges = true;

    io.DisplaySize.x = (float)window.GetWindowSize().x;
    io.DisplaySize.y = (float)window.GetWindowSize().y;

    ImGui::StyleColorsDark();


    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    double prevTime = 0, currentTime = 0, deltaTime = 0;
    uint32_t counter = 0;

    while (window.IsRunning()) {
        // ImGui Loop
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        ImGui_ImplIWindow_NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        currentTime = window.GetTime();
        deltaTime = currentTime - prevTime;
        counter++;
        if (deltaTime >= 1.0 / 1.0) {
            std::string FPS = std::to_string((int64_t)((1.0 / deltaTime) * counter));
            window.SetTitle(std::string{"IWindow OpenGL Example | FPS: "} + FPS);
        }
        prevTime = currentTime;



        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGui::EndFrame();

        window.Update();

        glcontext.SwapBuffers();
        gp.Update();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplIWindow_Shutdown();
    ImGui::DestroyContext();
}