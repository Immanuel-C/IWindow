# Vulkan with IWindow

IWindow needs some isntance extensions to create a `VkSurfaceKHR`. On Win32 the extensions are `VK_KHR_WIN32_SURFACE_EXTENSION_NAME` and `VK_KHR_SURFACE_EXTENSION_NAME`. On X11 the extensions are `VK_KHR_XLIB_SURFACE_EXTENSION_NAME` and `VK_KHR_SURFACE_EXTENSION_NAME`.

To get the required extensions IWindow has a function in `IWindowVK.h`.

`void IWindow::Vk::GetRequiredInstanceExtensions(std::vector<const char*>& extentionNames)`

Example:
```cpp
    ...

    std::vector<const char*> iWindowExtensionNames;
    IWindow::Vk::GetRequiredInstanceExtensions(iWindowExtensionNames);

    VkInstanceCreateInfo instanceInfo{};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = (uint32_t)iWindowExtensionNames.size();
    instanceInfo.ppEnabledExtensionNames = iWindowExtensionNames.data();

    ...
```

IWindow will create a `VkSurfaceKHR` for you since its platform dependent.

`VkResult IWindow::Vk::CreateSurface(VkInstance instance, VkSurfaceKHR& surface, Window& window)`.

This is pretty self explanitory. The function takes in an instance, a surface that will be modified and a `IWindow::Window&`. The function will return the output of the vk create function.