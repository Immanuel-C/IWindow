// Include glad or any other function loader before IWindow
#include <glad/glad.h>
#include "IWindow.h"
#include "IWindowWindow.h"
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

void MouseEnteredCallback(IWindow::Window& window, bool entered) {
    std::cout << std::boolalpha << "Mouse entered: " << entered << std::noboolalpha << '\n';
}

void WindowFucosCallback(IWindow::Window& window, bool focused) {
    std::cout << std::boolalpha << "Window is focused: " << focused << std::noboolalpha << '\n';
}

void PathDropCallback(IWindow::Window& window, std::vector<std::wstring>& paths, IWindow::Vector2<int32_t> mousePosition) {
    for (const std::wstring& path : paths)
        std::wcout << L"Path dropped: " << path << L'\n';
}

void FramebufferSizeCallback(IWindow::Window& window, IWindow::Vector2<int32_t> size) {
    glViewport(0, 0, (int)size.x , (int)size.y);
}

void APIENTRY DebugMessenger(GLenum source​, GLenum type​, GLuint id​,
    GLenum severity​, GLsizei length​, const GLchar* message​, const void* userParam​) {
    // Dont care.
    if (severity​ == GL_DEBUG_SEVERITY_NOTIFICATION) return;

    std::string sourceStr = "";
    std::string typeStr = "";
    std::string severityStr = "";

    switch (source​) {
        case GL_DEBUG_SOURCE_API:
            sourceStr = "GL_DEBUG_SOURCE_API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            sourceStr = "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            sourceStr = "GL_DEBUG_SOURCE_SHADER_COMPILER";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            sourceStr = "GL_DEBUG_SOURCE_THIRD_PARTY";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            sourceStr = "GL_DEBUG_SOURCE_APPLICATION";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            sourceStr = "GL_DEBUG_SOURCE_OTHER";
            break;
        default:
            break;
    }


    switch (type​) {
        case GL_DEBUG_TYPE_ERROR:
            typeStr = "GL_DEBUG_TYPE_ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            typeStr = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            typeStr = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            typeStr = "GL_DEBUG_TYPE_PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            typeStr = "GL_DEBUG_TYPE_PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_MARKER:
            typeStr = "GL_DEBUG_TYPE_MARKER";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            typeStr = "GL_DEBUG_TYPE_PUSH_GROUP";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            typeStr = "GL_DEBUG_TYPE_POP_GROUP";
            break;
        case GL_DEBUG_TYPE_OTHER:
            typeStr = "GL_DEBUG_TYPE_OTHER";
            break;
        default:
            break;
    }

    switch (severity​)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        severityStr = "GL_DEBUG_SEVERITY_HIGH";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        severityStr = "GL_DEBUG_SEVERITY_MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        severityStr = "GL_DEBUG_SEVERITY_LOW";
        break;
    default:
        break;
    }

    IWindow::Window* window = (IWindow::Window*)userParam​;


    std::wstring wTitle = window->GetTitle();

    std::string utf8Title{};
    utf8Title.resize(wTitle.size());

    for (size_t i = 0; i < wTitle.size(); i++) 
        utf8Title[i] = (char)wTitle[i];

    std::cout << "OpenGL context attached to window: " << utf8Title << " generated a message\n";
    std::cout << "Source: " << sourceStr << "\nType: " << typeStr << "\nSeverity: " << severityStr << "\nMessage: " << message​ << '\n';
}

/// Return true always.
bool IWindowErrorCallback(const IWindow::Error& error) {
    std::cout <<
        "IWindow error callback: " <<
        IWindow::ErrorTypeToString(error.type) << ' ' <<
        IWindow::ErrorSeverityToString(error.severity) << '\n' <<
        "Message: " << error.message << '\n';
        
        
    return true;
}

int main() {
    IWindow::Initialize(IWindow::CurrentVersion);
    // Or
    // IWindow::Initialize(IWindow::V100);

    IWindow::SetErrorCallback(IWindowErrorCallback);

    IWindow::Window window{};
    IWindow::GL::Context glcontext{};
    IWindow::Gamepad gp{ IWindow::GamepadID::GP1 };

    std::vector<IWindow::Monitor> monitors = IWindow::Monitor::GetAllMonitors();

    if (!window.Create({ WINDOW_WIDTH, WINDOW_HEIGHT }, L"Hello IWindow! (UTF-16 Symbol: π)", monitors[0])) return EXIT_FAILURE;
    
    IWindow::GL::ContextCreateInfo contextCreateInfo{};
    // Default value: 4, 6
    contextCreateInfo.version = { 4, 6 };
    // Default value: IWindow::GL::API::OpenGL
    contextCreateInfo.api = IWindow::GL::API::OpenGL;
    // Default value: IWindow::GL::Profile::Compatibility
    contextCreateInfo.profile = IWindow::GL::Profile::Core;
    // Default value: false
    contextCreateInfo.debugMode = true;
    // Default value: false
    contextCreateInfo.noError = false;
    // Default value: true
    contextCreateInfo.doubleBuffer = true;
    // Default value: 8, 8, 8, 8
    contextCreateInfo.rgbaBits = { 8, 8, 8, 8 };
    // Default value: 24
    contextCreateInfo.depthBits = 24;
    // Default value: 8
    contextCreateInfo.stencilBits = 8;
    // Default value: 0
    contextCreateInfo.samples = 0;
    // Default value: false
    contextCreateInfo.sRGB = true;
    // Default value: false. Broken.
    contextCreateInfo.steroscopicRendering = false;


    if (!glcontext.Create(window, contextCreateInfo)) {
        std::cout << "Failed to create a IWindow OpenGL context!\nThis is probably because your computer doesn't support the required version of OpenGL\n";
        return EXIT_FAILURE;
    }
    
    if (!gladLoadGL()) {
        std::cout << "Failed to load glad!\n";
        return EXIT_FAILURE;
    }

    glcontext.MakeContextCurrent(true);
    glcontext.vSync(true);

    if (contextCreateInfo.samples > 0)
        glEnable(GL_MULTISAMPLE);
    if (contextCreateInfo.sRGB)
        glEnable(GL_FRAMEBUFFER_SRGB);
    if (contextCreateInfo.debugMode && !contextCreateInfo.noError) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(DebugMessenger, &window);
    }
    window.SetMouseEnteredCallback(MouseEnteredCallback);
    window.SetWindowFocusCallback(WindowFucosCallback);
    window.SetPathDropCallback(PathDropCallback);
    window.SetFramebufferSizeCallback(FramebufferSizeCallback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplOpenGL3_Init("#version 450 core");
    ImGui_ImplIWindow_Init(window);

    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.ConfigWindowsResizeFromEdges = true;

    io.DisplaySize.x = (float)window.GetWindowSize().x;
    io.DisplaySize.y = (float)window.GetWindowSize().y;

    ImGui::StyleColorsDark();


    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
    
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

    for (IWindow::Monitor& monitor : monitors) {
        std::cout << "Name: " << monitor.name << ", x: " << monitor.position.x << " y: " << monitor.position.y << " Width: " << monitor.size.x << " Height: " << monitor.size.y << '\n';
    }

    // window.Fullscreen(true, monitors[0]);

    window.SetClipboardText("IWindow set the clipboard to this!");

    std::cout << "Clipboard Text: " << window.GetClipboardText() << '\n';

    int32_t polygonLineMode = GL_FILL;

    bool open = true;
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    while (window.IsRunning()) {
        static double framesPerSecond = 0.0;
        static int fps;
        static double lastTime = 0.0;
        double currentTime = window.GetTime() * 0.001;
        ++framesPerSecond;
        if (currentTime - lastTime > 1.0)
        {
            lastTime = currentTime;
            fps = (int)framesPerSecond;
            framesPerSecond = 0;
        }

        // ImGui Loop
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        ImGui_ImplIWindow_NewFrame();
        ImGui::ShowDemoWindow();

        if (ImGui::Begin("Example: Simple overlay", &open, windowFlags))
        {
            ImGui::Text("Frames Per Second");
            ImGui::Separator();
            ImGui::Text("FPS: %i", fps);
        }
        ImGui::End();

        ImGui::Render();

        if (window.IsKeyDown(IWindow::Key::M)) {
            window.SetMousePosition({});
        }

        if (window.IsKeyJustPressed(IWindow::Key::W)) {
            polygonLineMode = polygonLineMode == GL_FILL ? GL_LINE : GL_FILL;
            glPolygonMode(GL_FRONT_AND_BACK, polygonLineMode);
        }


        glClearColor(0.5f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        if (window.IsKeyJustPressed(IWindow::Key::Escape)) break;


        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGui::EndFrame();


        glcontext.SwapFramebuffers();
        window.Update();
        gp.Update();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplIWindow_Shutdown();
    ImGui::DestroyContext();

    glcontext.Destroy();
    window.Destroy();
    IWindow::Shutdown();
}