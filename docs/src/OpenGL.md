# OpenGL with IWindow

IWindow will handle creating the OpenGL context with the `GL::Context` class and `LoadOpenGLFunction(const char* name)`.

``` cpp
// in IWindowGL.h
namespace IWindow {
    namespace GL {
        class Context {
        public:
            Context() = default;
            Context(Window& window, uint16_t majorVersion, uint16_t minorVersion);
            ~Context();
            
            bool Create(Window& window, uint16_t majorVersion, uint16_t minorVersion);

            void MakeContextNotCurrent();
            void MakeContextCurrent();

            void SwapBuffers();

            void operator=(Context&) = delete;
            Context(Context&) = delete;
        };

        void* LoadOpenGLFunction(const char* name);
    }
}
```

The context class is less complecated than than the window class.

## Main Functions

like the window class the Context class has two ways you can create it with and the same rules that applied to the window class applies here.

Example:
```cpp
    ...
    IWindow::GL::Context glContext;

    if (!glContext.Create(...)) HandleError();

    ...

    while(window.IsRunning()) {
        ...
        window.Update();
    }

    ...
```

`void MakeContextCurrent()` tells opengl to render to the window passed into the `Create` function or the constructor. This function is called in the `Create` function/the constructor. 

`void SwapBuffers()` Swaps the front and back framebuffers should be called every frame.

Example:
```cpp
    ...
    while(window.IsRunning()) {
        window.Update();
        glContext.SwapBuffers();
    }
    ...
```

## Advanced Functions

`void* LoadOpenGLFunction(const char* name)` loads a function from opengl the dlls.