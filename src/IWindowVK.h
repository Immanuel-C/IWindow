#pragma once

#include "IWindow.h"

#include <vulkan/vulkan.h>

namespace IWindow {
    namespace Vk {
        void GetRequiredInstanceExtensions(std::vector<const char*>& extensionNames);
        VkResult CreateSurface(VkInstance instance, VkSurfaceKHR& surface, Window& window);
    }
}