#pragma once

#include "IWindow.h"


#include <vulkan/vulkan.hpp>
#if defined(_WIN32)
#include <vulkan/vulkan_win32.h>
#endif 

namespace IWindow {
    namespace Vk {
        void GetRequiredInstanceExtensions(std::vector<const char*>& extensionNames);
        VkResult CreateSurface(VkInstance instance, VkSurfaceKHR& surface, Window& window);
    }
}