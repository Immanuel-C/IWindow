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
        class IWINDOW_API Context {
        public:
            Context() = default;
            Context(Window& window, uint16_t majorVersion, uint16_t minorVersion);
            ~Context();
            
            bool Create(Window& window, uint16_t majorVersion, uint16_t minorVersion);

            void MakeContextNotCurrent();
            void MakeContextCurrent();
             
            void SwapBuffers();

            void vSync(bool vSync);

            void operator=(Context&) = delete;
            void operator=(Context&&) = delete;
            Context(Context&) = delete;
            Context(Context&&) = delete;
        private:
            Window* m_window;
            NativeGLRendereringContext m_rendereringContext;
        };

        void* LoadOpenGLFunction(const char* name);
    }
}