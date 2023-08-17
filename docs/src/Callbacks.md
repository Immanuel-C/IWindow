# Callbacks In IWindow

A callback is a function that is called during a certain event.

## Window Callbacks

#### Window Position Callback

The window position callback is called when the window is being moved;

- The `IWindow::Window&` argument is the window that called the callback. 
- The first `int64_t` argument is the new x position of the window.
- The second `int64_t` argument is the new y position of the window.

The internal x and y position will be automatically updated. So there is no need to call `window.SetPosition` on the window.

To register a window position callback, call `window.SetPosCallback(callback)`. Where window is a IWindow::Window.

An example of a window position callback is:

```cpp
void WindowPosCallback(IWindow::Window& window, int64_t x, int64_t y) {
    std::cout << "Window position: " << x << ", " << y << '\n';
}

...

This function returns the previous callback when a new one is installed.

// IWindow::Window
window.SetPosCallback(WindowPosCallback);
```

This function returns the previous callback when a new one is installed.

#### Window Size Callback

The window size callback is called when the window is being resized;

- The `IWindow::Window&` argument is the window that called the callback. 
- The first `int64_t` argument is the new width position of the window.
- The second `int64_t` argument is the new height position of the window.

The internal width and height position will be automatically updated. So there is no need to call `window.SetSize` on the window.

To register a window size callback, call `window.SetSizeCallback(callback)`. Where window is a IWindow::Window.

An example of a window size callback is:

```cpp
void WindowSizeCallback(IWindow::Window& window, int64_t width, int64_t height) {
    std::cout << "Window size: " << width << ", " << height << '\n';
}

...

// window is a IWindow::Window
window.SetSizeCallback(WindowPosCallback);
```

This function returns the previous callback when a new one is installed.

#### Window Focus Callback

The window focus callback is called when the window changes focus state (Focused/Unfocused).

- The `Window::Window&` argument is the window that called the callback.
= the `bool` argument tells you if the window has just come into focus (true) or just come out of focus (false).

To register a window focus callback, call `window.SetWindowFocusCallback()`. Where window is IWindow::Window.

An example of a WindowFocus callback is
```cpp
void FocusCallback(IWindow::Window& window, bool focused) {
    std::cout << "Window is focused: " << focused << '\n';
}



...

window.SetWindowFocusCallback(FocusCallback);
```

This function returns the previous callback when a new one is installed.

#### Framebuffer Size Callback

The framebuffer size callback is called when the framebuffer is being resized;

- The `IWindow::Window&` argument is the window that called the callback. 
- The first `int64_t` argument is the new width of the framebuffer.
- The second `int64_t` argument is the new height of the frambuffer.


To register a framebuffer size callback, call `window.SetFramebufferSizeCallback(callback)`. Where window is a IWindow::Window.

An example of a window size callback is:

```cpp
void FramebufferSizeCallback(IWindow::Window& window, int64_t width, int64_t height) {
    std::cout << "Framebuffer size: " << width << ", " << height << '\n';
}

...

// window is a IWindow::Window
window.SetFramebufferSizeCallback(WindowPosCallback);
```

This function returns the previous callback when a new one is installed.

## Input Callbacks

#### Key Callback

The key callback is called when a key is pressed on the key board.

- The `IWindow::Window&` argument is the window that called the callback. 
- The `IWindow::Key` argument is the key that was pressed (e.g. `Key::A`, `Key::B`, etc.)
- The `IWindow::InputState` argument says if the key was pressed or released.

to register a key callback, call `window.SetKeyCallback()`. Where window is a IWindow::Window.

An example of a key callback is:

```cpp
void KeyCallback(IWindow::Window& window, IWindow::Key key, IWindow::InputState state) {
    std::cout << "Key " << key << " was just " << state << '\n';
}

...

window.SetKeyCallback(KeyCallback);
```

This function returns the previous callback when a new one is installed.

#### Mouse Button Callback

The mouse button is called when a mouse button is pressed.

- The `IWindow::Window&` is the window that called the callback.
- The `IWindow::MouseButton` is what button was pressed.
- The `IWindow::InputState` is what input state (e.g. Down, Up) the button is in.
- The `IWindow::ClickState` is what click state the button is in (e.g. single clicked, double clicked).

to register a mouse button callback, call `window.SetMouseButtonCallback()`. Where window is a `IWindow::Window`.

An example of a mouse button callback is:

```cpp
void MouseButtonCallback(IWindow::Window& window, IWindow::MouseButton button, IWindow::InputState iState, IWindow::ClickState cState) {

}

...

window.SetMouseButtonCallback(MouseButtonCallback);

```

This function returns the previous callback when a new one is installed.

#### Mouse Move Callback

The mouse move callback is called when the cursor moves.

- The `IWindow::Window&` argument is the window that called the callback.
- The first `int64_t` argument is the cursors x position.
- The second `int64_t` argument is the cursors y position.

to register a mouse move callback for callback, call `window.SetMouseMoveCallback()`. Where window is a `IWindow::Window`.

An example of a mouse move callback is:

```cpp
void MouseMoveCallback(IWindow::Window& window, int64_t x, int64_t y) {
    std::cout << "Mouse Moved: " << x << ", " << y << '\n';
}

...

// window = IWindow::Window
window.SetMouseMoveCallback(MouseMoveCallback);

```

This function returns the previous callback when a new one is installed.

#### Mouse Scroll Callback

The mouse scroll callback is called when the mouse scoll wheel is scrolled.

- The `IWindow::Window&` argument is the window that called the callback.
- the first `float` argument is the scroll offset on the x axis.
- the second `float` argument is the scroll offset on the y axis.

The offsets will be 1 when scrolling forwards and -1 when scrolling backwards (towards the user).

An example of a mouse scroll callback is:

```cpp
void MouseScrollCallback(IWindow::Window& window, float xOffset, float yOffset) {
    std::cout << "xOffset: " << xOffset << ", yOffset: " << yOffset << '\n';
}

...

// window = IWindow::Window
window.SetMouseScrollCallback(MouseScrollCallback);
```

This function returns the previous callback when a new one is installed.

###

#### Mouse Entered Callback

The mouse entered callback is called when the mouse cursor enters or exits the windows borders.

- The `IWindow::Window&` argument is the window that called the callback.
- The `bool` argument is set to `true` when the mouse is entered and `false` when the mouse leaves.

To register the mouse entered callback call `window.SetMouseEnteredCallback()`. Where window is type `IWindow::Window`.

This function returns the previous callback when a new one is installed.

#### Char Callback

The char callback is similar to the key callback but instead of returning a keycode of the key pressed it returns a wide char that contains the keys ascii number.

- The `IWindow::Window&` argument is the window that called the callback.
- The `uint16_t` argument is a wide char that contains the ascii number of the key pressed.

To register the char callback call `window.SetCharCallback()`, Where window is type `IWindow::Window`

This function returns the previous callback when a new one is installed.


#### Gamepad Connected Callback

The gamepad connected callback is called when a gamepad is connected. 

- The `IWindow::GamepadID` argument is the ID of the gamepad that was connected/disconnected.
- The `bool` argument is if the gamepad was connected/disconnected. Connected if true and disconnected if false.

to register a gamepad connected callback, call `IWindow::Gamepad::SetConnectedCallback()`. The callback will be set for every gamepad thats why its static.

An example of a gamepad connected callback is:

```cpp
void GamepadConnectedCallback(IWindow::GamepadID gid, bool isConnected) {
    const char* sConnected = "";

    switch (isConnected)
    {
    case true:
        sConnected = "connected";
        break;
    case false:
        sConnected = "disconnected";
    default:
        break;
    }


    std::cout << "Gamepad " << (int)gid << " was " << sConnected << "!\n";
}

...

IWindow::Gamepad::SetConnectedCallback(GamepadConnectedCallback);
```