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
#include "IWindowPlatform.h"
#include "IWindowCore.h"


#include <iostream>

namespace IWindow {
    namespace GL {
        /// <summary>
        /// Specifies which profile mode the OpenGL context will created in.
        /// </summary>
        enum struct Profile {
            Compatibility,
            Core,
            Max
        };

        /// <summary>
        /// Always specify OpenGL.
        /// Reserved.
        /// </summary>
        enum struct API {
            OpenGL,
            Max
        };

        /// <summary>
        /// Information to create a OpenGL context.
        /// version the x component is the major version and the y component is the minor version.
        /// Api Reserved. Must be OpenGL.
        /// What profile the OpenGL context will be created with.
        /// doubleBuffer create a back and front framebuffer that are swapped when IWindow::GL::Context::SwapBuffers is called.
        /// steroscopicRendering enable left and right framebuffers. Currently broken.
        /// debugMode enables debug features such as the debug messenger. See https://registry.khronos.org/OpenGL/extensions/KHR/KHR_debug.txt.
        /// noError disables all debug features. GL_OUT_OF_MEMORY are still sent on devices that dont crash when they are out of memory. See https://registry.khronos.org/OpenGL/extensions/KHR/KHR_no_error.txt.
        /// sRgb allows the use of sRgb framebuffers.
        /// forwardCompatibility all deprecated functions are removed from the OpenGL version is 3.0+. The profile must be IWindow::Profile::Core if this is enabled.
        /// samples the amount of samples taken for multisampling.
        /// rgbaBits The size of the framebuffer colour attachment. Each component is a value in a vector4. The size is represented as bits.
        /// depthBits The size of the framebuffer depth attachment in a single int32_t. The sum of all the components is the depth buffer size. The size is represented as bits.
        /// stencilBits The size of the framebuffer stencil attachment in a single int32_t. The sum of all the components is the stencil buffer size. The size is represented as bits.
        /// </summary>
        struct ContextCreateInfo {
            Vector2<int32_t> version = { 4, 6 };
            API api = API::OpenGL;
            Profile profile = Profile::Compatibility;
            bool doubleBuffer = true;
            bool steroscopicRendering = false;
            bool debugMode = false;
            bool noError = false;
            bool sRGB = false;
            bool forwardCompatibility = false;
            int32_t samples = 0;
            // Framebuffer info.
            Vector4<int32_t> rgbaBits = { 8, 8, 8, 8 };
            int32_t depthBits = 24;
            int32_t stencilBits = 8;
        };

        /// <summary>
        /// a wrapper around an OpenGL context.
        /// </summary>
        class IWINDOW_API Context {
        public:
            Context() = default;
            /// <summary>
            /// Creates an OpenGL context. Perfer to use Create.
            /// </summary>
            /// <param name="window">The window this context will draw to.</param>
            /// <param name="contextCreateInfo">Information on how this OpenGL context should be created.</param>
            Context(Window& window, const ContextCreateInfo& contextCreateInfo = {});
            /// <summary>
            /// Destroy the OpenGL context.
            /// </summary>
            void Destroy();
            /// <summary>
            /// Creates an OpenGL context. 
            /// </summary>
            /// <param name="window">The window this context will draw to.</param>
            /// <param name="contextCreateInfo">Information on how this OpenGL context should be created.</param>
            /// <returns>
            /// true if the function succeeded.
            /// false if the function failed.
            /// </returns>
            bool Create(Window& window, const ContextCreateInfo& contextCreateInfo = {});
            /// <summary>
            /// Make this context current or not current.
            /// </summary>
            /// <param name="current">
            /// Set true if you are making the context current.
            /// Set false if you are making the context not current.
            /// </param>
            void MakeContextCurrent(bool current) const;
            /// <summary>
            /// Swap the framebuffers. Call this every frame.
            /// </summary>
            void SwapFramebuffers() const;
            /// <summary>
            /// Enable or disable v-sync. 
            /// </summary>
            /// <param name="vSync">
            /// Set true to enable v-sync.
            /// Set false to disable v-sync.
            /// </param>
            void vSync(bool vSync) const;
            /// <summary>
            /// Load an OpenGL function from the driver.
            /// </summary>
            /// <param name="name">Name of the function to load.</param>
            /// <returns>The function that was requested. The function returns nullptr if it failed.</returns>
            static void* LoadOpenGLFunction(const char* name);

            void operator=(Context&) = delete;
            Context(Context&) = delete;
        private:
            Window* m_window;
            NativeGLRendereringContext m_rendereringContext;
        };


    }
}