#ifdef _WIN32

#include "IWindowVK.h"

#include <vulkan/vulkan_win32.h>

namespace IWindow {
    namespace Vk {
        void GetRequiredInstanceExtensions(std::vector<const char*>& extensionNames) {
            extensionNames.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
            extensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
        }

        VkResult CreateSurface(VkInstance instance, VkSurfaceKHR& surface, Window& window) {
            VkWin32SurfaceCreateInfoKHR surfaceInfo{};
            surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
            surfaceInfo.hinstance = GetModuleHandle(nullptr);
            surfaceInfo.hwnd = window.GetNativeWindowHandle();

            return vkCreateWin32SurfaceKHR(instance, &surfaceInfo, nullptr, &surface);
        }
    }
}

#endif