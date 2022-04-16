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
            bool Create(Window& window, uint16_t majorVersion, uint16_t minorVersion);

            void MakeContextNotCurrent();

            void SwapBuffers();
        private:
            Window* m_window;
            NativeGLRendereringContext m_rendereringContext;
        };

        void* LoadOpenGLFunction(const char* name);
    }
}