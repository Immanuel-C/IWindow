/*
    BSD 2-Clause License

    Copyright (c) 2022, Immanuel Charles
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once

#include "IWindow.h"
#include "IWindowCore.h"

#include <vulkan/vulkan.h>

namespace IWindow {
    namespace Vk {
        /// <summary>
        /// Get the Vulkan instance extensions IWindow requires.
        /// </summary>
        /// <param name="extensionNames">This funciton will push back all the required extensions to this vector.</param>
        void IWINDOW_API GetRequiredInstanceExtensions(std::vector<const char*>& extensionNames);
        /// <summary>
        /// Creates a VkSurface.
        /// </summary>
        /// <param name="window">The window Vulkan will draw too.</param>
        /// <param name="instance">A valid VkInstance.</param>
        /// <param name="surface">The VkSurfaceKHR handle that IWindow will store the surface in.</param>
        /// <returns>Return the value of the vkCreateXXXXSurfaceKHR function.</returns>
        VkResult IWINDOW_API CreateSurface(Window& window, VkInstance& instance, VkSurfaceKHR& surface);
    }
}
