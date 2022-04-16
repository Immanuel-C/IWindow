#pragma once

// TODO(IC): Implemented this
#include "IWindow.h"
#include "IWindowPlatform.h"


namespace IWindow {
    namespace GL {
        class Context {
        public:
            Context() = default;
            Context(Window& window, uint16_t majorVersion, uint16_t minorVersion);
            ~Context();
            
            bool Create(Window& window, uint16_t majorVersion, uint16_t minorVersion);

            void MakeContextNotCurrent();
            void MakeContextCurrent();

            void SwapBuffers();

            Window* m_window;
            NativeGLRendereringContext m_rendereringContext;

            void operator=(Context&) = delete;
            Context(Context&) = delete;
        };

        void* LoadOpenGLFunction(const char* name);
    }
}