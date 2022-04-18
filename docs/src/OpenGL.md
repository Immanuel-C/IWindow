# OpenGL with IWindow

IWindow will handle creating the OpenGL context with the `IWindow::GL::Context` class and `IWindow::GL::LoadOpenGLFunction(const char* name)`.

everything in this page is in the `IWindowGL.h` header file.

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

like the window class the Context class has two ways you can create it with and the same rules that applied to the window class applies here. These two ways are the constructor and the `Create` function. `Create` already makes the OpenGL context current.

After you create the context you can then initialize any opengl function loaders like glad and glew. 

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

`void IWindow::GL::Context::MakeContextCurrent()` makes the OpenGL renderering context to be current.

`void IWindow::GL::Context::SwapBuffers()` Swaps the front and back framebuffers should be called every frame.

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

`void* IWindow::GL::LoadOpenGLFunction(const char* name)` loads a function from OpenGL the dlls.