# What is IWindow

IWindow is a windowing library that is meant to be used with Vulkan or OpenGL. 

The goal of IWindow is:
    
- IWindow should be cross-platform and work with Win32 (Windows) and X11 (Linux) windowing apis but currently there is only a Win32 version.  

# How can I build IWindow?

IWindow uses the [premake5](https://premake.github.io/) build system.

IWindow comes with premade premake projects to build easily to a static library. These projects are `IWindowWin32GL` and `IWindowWin32Vk`. As the name suggests GL is for OpenGL projects and Vk is for Vulkan. There is also a `IWindowWin32All` project for OpenGL and Vulkan. 

# Important Stuff You Should Know About

All functions/classes are in the `IWindow` namespace.