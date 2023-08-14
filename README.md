# IWindow

### **IWindow is currently in pre-alpha, stability is not guaranteed!** 

A simple windowing library ment to be used with Vulkan, OpenGL or Direct3D.
 
IWindow is written is C++ and uses C++17 and currently only supports 64 bit machines.
 
IWindow supports Win32 (Windows) and X11 (Most Linux machines).
 
No this isn't as good as GLFW yet...

# IWindow Features

- Create a window
- Create an OpenGL context (no OpenGL ES support)
- Create a Vulkan surface and get the required instance extensions for IWindow to link with Vulkan
- Keyboard and mouse support with callbacks
- Gamepad support with a gamepad connect callback (Linux doesn't support rumble yet because of bugs and some features are missing like setting the deadzone)
- Win32 (Windows) and X11 (Most Linux machines) machines are supported

# How to build The Example

Use premake5. If you dont have premake5 you can install it [here](https://premake.github.io/). 
Add the premake5 executable into the main folder or into your environment path.

Run the build.bash on Linux or if your on Windows you can use the build.bat if you have visual studio 2022. If you dont have visual studio 2022 you can use `premake5 [build target] [args]`.

The documentation of this library is on this [website](https://immanuel-c.github.io/IWindow).

# Contribute

See [Contribute](./Contribute.md).
