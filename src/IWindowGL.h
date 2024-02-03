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
        /// a wrapper around an OpenGL context.
        /// </summary>
        class IWINDOW_API Context {
        public:
            Context() = default;
            /// <summary>
            /// Creates an OpenGL context. Perfer to use Create.
            /// </summary>
            /// <param name="window">The window this context will draw to.</param>
            /// <param name="majorVersion">Major OpenGL version that this context will initialize.</param>
            /// <param name="minorVersion">Minor OpenGL version that this context will initialize.</param>
            Context(Window& window, uint16_t majorVersion, uint16_t minorVersion);
            /// <summary>
            /// Destroy the OpenGL context.
            /// </summary>
            void Destroy();
            /// <summary>
            /// Creates an OpenGL context. 
            /// </summary>
            /// <param name="window">The window this context will draw to.</param>
            /// <param name="majorVersion">Major OpenGL version that this context will initialize.</param>
            /// <param name="minorVersion">Minor OpenGL version that this context will initialize.</param>
            /// <returns>
            /// true if the function succeeded.
            /// false if the function failed.
            /// </returns>
            bool Create(Window& window, uint16_t majorVersion, uint16_t minorVersion);
            /// <summary>
            /// Make this context current or not current.
            /// </summary>
            /// <param name="current">
            /// Set true if you are making the context current.
            /// Set false if you are making the context not current.
            /// </param>
            void MakeContextCurrent(bool current) const;
            /// <summary>
            /// Swap the framebuffers.
            /// </summary>
            void SwapBuffers() const;
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