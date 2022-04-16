#include "IWindowVK.h"


void IWindow::Vk::GetRequiredInstanceExtensions(std::vector<const char*>& extensionNames)
{
    extensionNames.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    extensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
}

VkResult IWindow::Vk::CreateSurface(VkInstance instance, VkSurfaceKHR& surface, Window& window)
{
    VkWin32SurfaceCreateInfoKHR surfaceInfo{};
    surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.hinstance = GetModuleHandleA(0);
    surfaceInfo.hwnd = window.GetNativeWindowHandle();

    return vkCreateWin32SurfaceKHR(instance, &surfaceInfo, nullptr, &surface);
}
