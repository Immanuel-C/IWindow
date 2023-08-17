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
        // Add
        IVector2 GetFramebufferSize();

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

        bool IsMouseButtonUp(MouseButton button);
        template<typename... Args>
        bool IsMouseButtonUp(MouseButton button, Args... args) { return IsMouseButtonUp(button) && IsMouseButtonUp(args...); }

        Vector2 GetMouseScrollOffset();

        void SetUserPointer(void* ptr);
        void* GetUserPointer();
        
        WindowPosCallback SetPosCallback(WindowPosCallback callback);
        WindowSizeCallback SetSizeCallback(WindowSizeCallback callback);
        KeyCallback SetKeyCallback(KeyCallback callback);
        MouseMoveCallback SetMouseMoveCallback(MouseMoveCallback callback);
        MouseButtonCallback SetMouseButtonCallback(MouseButtonCallback callback);
        MouseScrollCallback SetMouseScrollCallback(MouseScrollCallback callback);
        WindowFocusCallback SetWindowFocusCallback(WindowFocusCallback callback);
        MouseEnteredCallback SetMouseEnteredCallback(MouseEnteredCallback callback);
        CharCallback SetCharCallback(CharCallback callback);
        FramebufferSizeCallback SetFramebufferSizeCallback(FramebufferSizeCallback callback);

        Monitor GetPrimaryMonitor();
        std::vector<Monitor> GetAllMonitors();

        void Center(Monitor monitor);
        void Fullscreen(bool fullscreen, Monitor monitor);
        bool IsFullscreen();

        void SetIcon(Image image);
        void SetCursor(Image image, uint32_t hotX, uint32_t hotY);
        void SetIcon(NativeIconID iconID);
        void SetCursor(NativeCursorID cursorID);

        std::string GetClipboardText();
        void SetClipboardText(const std::string& text);

        NativeDeviceContext& GetNativeDeviceContext();
        
        double GetTime();

        // Add
        bool IsFocused();

        void SetTitle(const std::string& title);

        void operator=(Window&) = delete;
        void operator=(Window&&) = delete;
        Window(Window&) = delete;
        Window(Window&&) = delete;
    private:
        ...
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

`std::string GetClipboardText()` returns the text currently in the clipboard.
`void SetClipboardText(const std::string& text)` sets the text in the clipboard. 

`double GetTime()` returns time since window was initialized.

`void SetTitle(const std::string& title)` Sets the title of the window.

`void IWindow::Window::SetIcon(Image image)` sets the window icon to an image. The image has to have 4 channels, in order `RGBA`, 8 bits/1 byte per channel. The image has to starts at the top left corner.

`void IWindow::Window::SetCursor(Image image, uint32_t hotX, uint32_t hotY)`. sets the cursor to an image. The cursor has to be inside the window to show image. The image has to have 4 channels, in order `RGBA`, 8 bits/1 byte per channel, the image has to start at the top left corner. `hotX` is the location of the x location that effects cursor events. `hotY` is similer but it is the y location. Think of the 'Hot' location has the place cursor events take place (e.g. when you click it will be at the hot location).


`void IWindow::Window::SetIcon(NativeIconID iconID)` sets the icon using a native icon id. See [Enum Structs](./EnumStructs.md).

`void IWindow::Window::SetCursor(NativeCursorID cursorID)` sets the cursor using a native cursor id. See [Enum Structs](./EnumStructs.md).

`bool IsFocused()` checks if the window is focused.

## Callbacks

The `void IWindow::Window::SetPosCallback`, `void IWindow::Window::SetKeyCallback`, etc. are called when an action like moving the window or a key is pressed happens. For more info go to the [Callbacks section](./Callbacks.md).


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

`bool IWindow::Window::IsKeyDown(IWindow::Window::Key key)` function checks if a key is pressed.

`bool IWindow::Window::IsMouseButtonDown(IWindow::MouseButton button)` function checks if a argument `button` is pressed. 

The templated input functions allow you to pass in multiple buttons/keys and the function checks if all the buttons/keys are pressed/released.

Example:

```cpp

...

IWindow::Window window;

...

// statment will be true if the left and right buttons are pressed
if (window.isMouseButtonDown(IWindow::MouseButton::Left, IWindow::MouseButton::Right)) {
    ...
}

...

```

`Vector2 IWindow::Window::GetMouseScrollOffset()` gets the scrolls offset. The offsets will be 1 when scrolling forwards and -1 when scrolling backwards (towards the user). See [Vectors](Vector.md).

## Monitors Related Functions

`IWindow::Monitor IWindow::Window::GetPrimaryMonitor()` gets the primary monitor on your computor. See [Monitors](./Monitor.md).

`std::vector<IWindow::Monitor> IWindow::Window::GetAllMonitors()` gets all the available monitors. See [Monitors](./Monitor.md).

`void IWindow::Window::Center(IWindow::Monitor monitor)` centers the window to the center of the monitor provided. See [Monitors](./Monitor.md).

`void IWindow::Window::Fullscreen(bool fullscreen, IWindow::Monitor monitor)` sets the window to fullscreen mode on the monitor provided if true and if false centers the window on the monitor provided and sets the windows width and height to the width and height of the window before fullscreen. See [Monitors](./Monitor.md).

`bool IWindow::Window::IsFullscreen()` return true if window is fullscreen and false if not.


## Advanced Functions

`IWindow::Window::NativeWindowHandle& IWindow::Window::GetNativeWindowHandle()` gets the internal windowing api's window handle (e.g. Win32: `HWND`).

`IWindow::Window::NativeDeviceContext& IWindow::Window::GetNativeDeviceContext()` gets the internal windowing api's graphics/device context (e.g. Win32: `HDC`).
