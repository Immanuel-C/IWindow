# Window Class

The window class is where the magic happens. The window class as the name suggests is the class that is reponsible for creating the window on your screen.

The window class is kept in the IWindow.h header file.

This is what the window class .h file looks like (with some stuff taken out).

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

        IVector2 GetWindowSize();
        IVector2 GetWindowPosition();

        IVector2 GetMousePosition();

        void SetWindowSize(int64_t width, int64_t height);
        void SetWindowPosition(int64_t x, int64_t y);

        bool IsKeyDown(Key key);
        template<typename... Args>
        bool IsKeyDown(Key key, Args... args) { return IsKeyDown(key) && IsKeyDown(args...); }

        bool IsKeyUp(Key key);
        template<typename... Args>
        bool IsKeyUp(Key key, Args... args) { return IsKeyUp(key) && IsKeyUp(args...); }

        bool IsMouseButtonDown(MouseButton button);
        template<typename... Args>
        bool IsMouseButtonDown(MouseButton button, Args... args) { return IsMouseButtonDown(button) && IsMouseButtonDown(args...); }

        bool IsMouseButtonDoubleClicked(MouseButton button);
        template<typename... Args>
        bool IsMouseButtonDoubleClicked(MouseButton button, Args... args) { return IsMouseButtonDoubleClicked(button) && IsMouseButtonDoubleClicked(args...); }

        bool IsMouseButtonUp(MouseButton button);
        template<typename... Args>
        bool IsMouseButtonUp(MouseButton button, Args... args) { return IsMouseButtonUp(button) && IsMouseButtonUp(args...); }

        void SetUserPointer(void* ptr);
        void* GetUserPointer();

        void SetPosCallback(WindowPosCallback callback);
        void SetSizeCallback(WindowSizeCallback callback);
        void SetKeyCallback(KeyCallback callback);
        void SetMouseMoveCallback(MouseMoveCallback callback);
        void SetMouseButtonCallback(MouseButtonCallback callback);

        void Center();
        void Fullscreen(bool fullscreen);
        bool IsFullscreen();

        void SetIcon(Image image);
        void SetCursor(Image image, uint32_t hotX, uint32_t hotY);
        void SetIcon(NativeIconID iconID);
        void SetCursor(NativeCursorID cursorID);

        NativeGLDeviceContext& GetNativeGLDeviceContext();
        
        void operator=(Window&) = delete;
        Window(Window&) = delete;
    };
}
```

Its a lot to take in right even though this is only the public stuff. Dont worry we will be going through everything in the window class.

## Main Functions

`bool IWindow::Window::Create(int64_t width, int64_t height, const std::string& title, int64_t x = 100, int64_t y = 100)` creates the window you could use the constructor (which takes in the same args) but `Create` returns a `false` if something fails. 

Example:
```cpp
    IWindow::Window window{};
    if (!window.Create(...)) return -1;
    
    // Or

    // Doesn't return bool if fails
    window = IWindow::Window(...);
```

`bool IWindow::Window::IsRunning()` checks if the window is still up and running. `IsRunning` with return `false` if a window is not running (e.g. when a user clicks the x in the top of the window isRunning will return `false`).

Example:
```cpp
while (window.IsRunning()) {
    // do stuff
}
```

`void IWindow::Window::Update()` checks for events like input or if the window moved and should be called every frame.

Example:
```cpp
    while(window.isRunning()) {
        // do stuff
        
        window.Update();
    }
```


## Getters And Setters
`IVector2 IWindow::Window::GetWindowSize()` returns a `IWindow::IVector2`. Where `x` is the width and `y` is the height. See [Vector](./Vector.md).

where `x` and `y` are width and height of the window.

`void IWindow::Window::SetWindowSize(int64_t width, int64_t height)` takes in 2 `int64_t` (64 bit integer) width, height. This function sets the width and height of the window.

`void IWindow::Window::SetWindowPosition(int64_t x, int64_t x)` takes in 2 `int64_t` (64 bit integer) x, y. This function sets the window position.

`IVector2 IWindow::Window::GetWindowPosition()` return a `IWindow::IVector2`. Where `x` and `y` are the window position. See [Vector](./Vector.md).

The `void IWindow::Window::SetPosCallback`, `void IWindow::Window::SetKeyCallback`, etc. are called when an action like moving the window or a key is pressed happens. For more info goto the [Callbacks section](./Callbacks.md).


Example:
```cpp
#include <iostream> // For std::cout

void WindowPosCallback(IWindow::Window& window, int64_t x, int64_t y) {
    int* example = (int*)window.GetUserPointer();
    std::cout << "User Pointer: " << example << '\n'; // Output: 10
    std::cout << "Window position: " << x << ", " << y << '\n';
}

An example of a `WindowPosCallback` is
```cpp
#include <iostream> // For std::cout

void WindowPosCallback(IWindow::Window& window, int64_t x, int64_t y) {
    std::cout << "Window position: " << x << ", " << y << '\n';
}
```

`x` and `y` are the `x` and `y` coordinates of the window.

`void IWindow::Window::SetUserPointer(void* ptr)` sets an internal pointer that can be retreived in a callback or some where else.

```cpp
int main() {
    ...

    int example = 10;
    // window = IWindow::Window
    window.SetUserPointer(&example);
    window.SetPosCallback(WindowPosCallback);
    ...
}

```

`void* IWindow::Window::GetUserPointer()` gets the pointer set from `IWindow::Window::SetUserPointer`. `nullptr` if no pointer is set

[//]: <> (Add more to input)

## Input Functions

The `bool IWindow::Window::IsKeyDown(IWindow::Window::Key key)` and `IWindow::Window::IsKeyUp(IWindow::Window::Key key)` functions check if a key is pressed or is released. A key callback could be used to handle this.


`bool IWindow::Window::IsMouseButtonDown(IWindow::MouseButton button)` and `bool IWindow::Window::IsMouseButtonUp(IWindow::MouseButton button)` functions check if a argument `button` is pressed or released. 

`bool IWindow::Window::IsMouseButtonDoubleClicked(IWindow::MouseButton button)` checks if a argument `button` was pressed 2 in quick succesion.

The templated input functions allow you to pass in multiple buttons/keys and the function checks if all the buttons/keys are pressed/released.

Example:

```cpp
...
// statment will be true if the left and right buttons are pressed
// window = IWindow::Window
if (window.isMouseButtonDown(IWindow::MouseButton::Left, IWindow::MouseButton::Right)) {
    ...
}

...
```

`void IWindow::Window::Center()` centers the window to the center of the screen.

`void IWindow::Window::Fullscreen(bool fullscreen)` sets the window to fullscreen mode if true and if false centers the window and sets the windows width and height to the width and height of the window before fullscreen.

`bool IWindow::Window::IsFullscreen()` return true if window is fullscreen and false if not.

`void IWindow::Window::SetIcon(Image image)` sets the window icon to an image. The image has to have 4 channels, in order `RGBA`, 8 bits/1 byte per channel. The image has to starts at the top left corner.

`void IWindow::Window::SetCursor(Image image, uint32_t hotX, uint32_t hotY)`. sets the cursor to an image. The cursor has to be inside the window to show image. The image has to have 4 channels, in order `RGBA`, 8 bits/1 byte per channel, the image has to start at the top left corner. `hotX` is the location of the x location that effects cursor events. `hotY` is similer but it is the y location. Think of the 'Hot' location has the place cursor events take place (e.g. when you click it will be at the hot location).

`void IWindow::Window::SetIcon(NativeIconID iconID)` sets the icon using a native icon id. See [Enum Structs](./EnumStructs.md).

`void IWindow::Window::SetCursor(NativeCursorID cursorID)` sets the cursor using a native cursor id. See [Enum Structs](./EnumStructs.md).

## Advanced Functions

`NativeWindowHandle& IWindow::Window::GetNativeWindowHandle()` gets the internal windowing api's window handle (e.g. Win32: `HWND`, X11: `Window`).

`NativeGLDeviceContext& IWindow::Window::GetNativeGLDeviceContext()` gets the internal windowing api's graphics/device context (e.g. Win32: `HDC`, X11: `GC`).