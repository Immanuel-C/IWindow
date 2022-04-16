#pragma once

// TODO(IC): Implemented this
#include "IWindow.h"

namespace IWindow {
    namespace GL {
        class Context;
        void CreateAndAttachContext(Window& window, Context& context);
        void MakeContextCurrent(Window& window);
    }
}