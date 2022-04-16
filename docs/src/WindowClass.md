# Window Class

The window class is where the magic happens. The window class as the name suggests is the class that is reponsible for creating the window on your screen.

This is what the window class .h file looks like

```cpp
namespace IWindow {
    class Window {
    public:
        Window() = default;
        Window(int64_t width, int64_t height, const std::string& title, int64_t x = 100, int64_t y = 100);
        ~Window(); 

        bool Create(int64_t width, int64_t height, const std::string& title, int64_t x = 100, int64_t y = 100);

        void Update();

        bool IsRunning();

        NativeWindowHandle& GetNativeWindowHandle(); 

        WindowSize GetWindowSize();
        WindowPos GetWindowPosition();

        void SetWindowSize(int64_t width, int64_t height);
        void SetWindowPosiiton(int64_t x, int64_t y);

        bool IsKeyDown(Key key);
        template<typename... Args>
        bool IsKeyDown(Key key, Args... args) { return IsKeyDown(key) && IsKeyDown(args...); }

        bool IsKeyUp(Key key);
        template<typename... Args>
        bool IsKeyUp(Key key, Args... args) { return IsKeyUp(key) && IsKeyUp(args...); }

        void SetUserPointer(void* ptr);
        void* GetUserPointer();

        void SetPosCallback(WindowPosCallback callback);
        void SetSizeCallback(WindowSizeCallback callback);
        void SetKeyCallback(KeyCallback callback);

        NativeGLDeviceContext& GetNativeGLDeviceContext();
        
        void operator=(Window&) = delete;
        Window(Window&) = delete;
    };
}
```

Its a lot to take in right even though this is only the public stuff. Dont worry we will be going through everything in the window class.

## Main Functions

`bool Create(int64_t width, int64_t height, const std::string& title, int64_t x = 100, int64_t y = 100)` creates the window you could use the constructor (which takes in the same args) but `Create` returns a `false` if something fails. 

Example:
```cpp
    IWindow::Window window{};
    if (!window.Create(...)) return -1;
    
    // Or

    // Doesn't return bool if fails
    window = IWindow::Window(...);
```

`bool IsRunning()` checks if the window is still up and running. `IsRunning` with return `false` if a window is not running (e.g. when a user clicks the x in the top of the window isRunning will return `false`).

Example:
```cpp
while (window.IsRunning()) {
    // do stuff
}
```

`void Update()` checks for events like input or if the window moved and should be called every frame.

Example:
```cpp
    while(window.isRunning()) {
        // do stuff
        
        window.Update();
    }
```


## Getters And Setters
`WindowSize GetWindowSize()` returns a `WindowSize` which is
```cpp
    struct WindowSize {
        int64_t width, height;
    };
```

`void SetWindowSize(int64_t width, int64_t height)` takes in 2 `int64_t` (64 bit integer) width, height.


The `void SetPosCallback`, `void SetKeyCallback`, etc. are called when an action like moving the window or a key is pressed happens.

An example of a `WindowPosCallback` is
```cpp
#include <iostream> // For std::cout

void WindowPosCallback(IWindow::Window& window, int64_t x, int64_t y) {
    std::cout << "Window position: " << x << ", " << y << '\n';
}
```

`x` and `y` are the `x` and `y` coordinates of the window.

[//]: <> (Add more to input)

The `bool IsKeyDown(Key key)` and `IsKeyUp(Key key)` functions check if a key is pressed or is released. A key callback could be used to handle this. It takes in a Key look at [Input Enums](./InputEnums.md).

`void SetUserPointer(void* ptr)` sets an internal pointer that can be retreived in a callback or some where else.

`void* GetUserPointer()` gets the pointer set from `SetUserPointer`. `nullptr` if no pointer is set

Example:
```cpp
#include <iostream> // For std::cout

void WindowPosCallback(IWindow::Window& window, int64_t x, int64_t y) {
    int* example = (int*)window.GetUserPointer();
    std::cout << "User Pointer: " << example << '\n'; // Output: 10
    std::cout << "Window position: " << x << ", " << y << '\n';
}

int main() {
    ...

    int example = 10;
    window.SetUserPointer(&example);
    window.SetPosCallback(WindowPosCallback);
    ...
}

```

## Advanced Functions

`NativeWindowHandle& GetNativeWindowHandle()` gets the internal windowing api's window handle (e.g. Win32: `HWND`, X11: `Window`).

`NativeGLDeviceContext& GetNativeGLDeviceContext()` gets the internal windowing api's graphics/device context (e.g. Win32: `HDC`, X11: `GC`).