#if !defined(_WIN32)
#include "IWindowVK.h"

#include <vulkan/vulkan_xlib.h>


namespace IWindow {
    namespace Vk {
        void GetRequiredInstanceExtensions(std::vector<const char*>& extensionNames) {
            extensionNames.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
            extensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
        }

        VkResult CreateSurface(VkInstance instance, VkSurfaceKHR& surface, Window& window) {
            VkXlibSurfaceCreateInfoKHR surfaceInfo{};
            surfaceInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
            surfaceInfo.dpy = window.GetX11Display();
            surfaceInfo.window = window.GetNativeWindowHandle();

            return vkCreateXlibSurfaceKHR(instance, &surfaceInfo, nullptr, &surface);
        }
    }
}
#endif // _WIN32