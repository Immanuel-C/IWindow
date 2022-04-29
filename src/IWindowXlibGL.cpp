#include "IWindowGL.h"

#include <GL/gl.h>

namespace IWindow {
    namespace GL {

        void* LoadOpenGLFunction(const char* name) {
            void* fun = (void*)glXGetProcAddress((const GLubyte*)name);
            return fun;
        }
    }
}