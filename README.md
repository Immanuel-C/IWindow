# IWindow

## Documentation website is not up to date. Use header files documentation.

## IWindow does not support Linux anymore! ##

I decided to remove linux support of IWindow since it was too much work for 1 person to handle. You can still find Linux support in the OldLinuxSupport branch if others want to contribute to it. This doesn't mean IWindow will never support linux.

A simple windowing library ment to be used with Vulkan, OpenGL or Direct3D.
 
IWindow is written is C++ and uses C++17 and currently only supports 64 bit machines.
 
IWindow supports Win32 (Windows).
 
No this isn't as good as GLFW yet...

# IWindow Features

- Create a window
- Create an OpenGL context (no OpenGL ES support)
- Create a Vulkan surface and get the required instance extensions for IWindow to link with Vulkan
- Keyboard and mouse support with callbacks
- Gamepad support with a gamepad connect callback.
- Win32 (Windows).

# How to build The Example

Use premake5. If you dont have premake5 you can install it [here](https://premake.github.io/). 
Add the premake5 executable into the main folder or into your environment path.

on Windows you can use the build.bat if you have visual studio 2022. If you dont have visual studio 2022 you can use `premake5 [build target] [args]`.

The documentation of this library is on this [website](https://immanuel-c.github.io/IWindow).

# Building IWindow into a DLL

When compiling define `IWINDOW_BUILD_DLL` in preprocessor defines. Also use `IWINDOW_DLL` in preprocessor defines and client applications preprocessor defines.

# Contribute

See [Contribute](./Contribute.md).
