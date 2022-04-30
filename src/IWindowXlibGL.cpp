#if !defined(_WIN32)
#include "IWindowGL.h"

#include <GL/gl.h>
#include <iostream>

typedef GLXContext (*FNP_glXCreateContextAttribsARB)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

static int X11ErrorHandler(Display* display, XErrorEvent* ev) {
    std::cout << "X11 GLX Error!\nMajor Code: " << (int)ev->error_code << ", Minor Code: " << (int)ev->minor_code << '\n';
    return false;
}

namespace IWindow {
    namespace GL {

        Context::Context(Window& window, uint16_t majorVersion, uint16_t minorVersion) { Create(window, majorVersion, minorVersion); }
        Context::~Context() { glXDestroyContext(m_window->GetX11Display(), m_rendereringContext); }


        bool Context::Create(Window& window, uint16_t majorVersion, uint16_t minorVersion) {
            m_window = &window;

            static std::array<int, 23> visualAttribs = {
                GLX_X_RENDERABLE, true,
                GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                GLX_RENDER_TYPE, GLX_RGBA_BIT,
                GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
                GLX_RED_SIZE, 8,
                GLX_GREEN_SIZE, 8,
                GLX_BLUE_SIZE, 8,
                GLX_ALPHA_SIZE, 8,
                GLX_DEPTH_SIZE, 24,
                GLX_STENCIL_SIZE, 8,
                GLX_DOUBLEBUFFER, true,
                None
            };

            if ( (majorVersion == 1 && minorVersion < 3) 
            || majorVersion < 1) {
                std::cerr << "OpenGL Context version is less than 1.3!\n";
                return false;
            }

            int fbcount = 0;


            GLXFBConfig* fbc = glXChooseFBConfig(
                window.GetX11Display(), 
                DefaultScreen(window.GetX11Display()), 
                visualAttribs.data(), 
                &fbcount
            );

            if (!fbc) {
                std::cerr << "Failed to retrieve a framebuffer config\n";
                return false;
            }

            int bestFbcIndex = -1, worstFbcIndex = -1, BestNumSamples = -1, worstNumSamples = -1;

            // Choose framebuffer config
            for (int32_t i = 0; i < fbcount; i++) {
                XVisualInfo* visualInfo = glXGetVisualFromFBConfig(window.GetX11Display(), fbc[i]);
                if (visualInfo) {
                    int sampleBuffers = 0, samples = 0;
                    glXGetFBConfigAttrib(window.GetX11Display(), fbc[i], GLX_SAMPLE_BUFFERS, &sampleBuffers);
                    glXGetFBConfigAttrib(window.GetX11Display(), fbc[i], GLX_SAMPLES, &samples);
                    
                    if ( bestFbcIndex < 0 || sampleBuffers && samples > BestNumSamples )
                        bestFbcIndex = i, BestNumSamples = samples;
                    if ( worstFbcIndex < 0 || !sampleBuffers || samples < worstNumSamples )
                        worstFbcIndex = i, worstNumSamples = samples;
                }
                XFree(visualInfo);
            }


            GLXFBConfig bestFbc = fbc[bestFbcIndex];
            XVisualInfo *vi = glXGetVisualFromFBConfig( window.GetX11Display(), bestFbc );
            XFree(fbc);

            //int (*oldX11ErrorHandler)(Display*, XErrorEvent*) = XSetErrorHandler(X11ErrorHandler);

            FNP_glXCreateContextAttribsARB glXCreateContextAttribsARB = (FNP_glXCreateContextAttribsARB)LoadOpenGLFunction("glXCreateContextAttribsARB");

            if (!glXCreateContextAttribsARB) {
                std::cerr << "Failed to load glXCreateContextAttribsARB. This functions is required to create an OpenGL context!\n";
                return false;
            }

            std::array<const int, 7> contextAttribs = {
                GLX_CONTEXT_MAJOR_VERSION_ARB, majorVersion,
                GLX_CONTEXT_MINOR_VERSION_ARB, minorVersion,
                GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
                None
            };

            m_rendereringContext = 
            glXCreateContextAttribsARB(window.GetX11Display(), bestFbc, 0, true, contextAttribs.data());

            if (!m_rendereringContext) {
                std::cerr << "Failed to create OpenGL context!\n";
                return false;
            }

            if (!glXIsDirect(window.GetX11Display(), m_rendereringContext)) {
                std::cout << "OpenGL context is not direct!\n";
            }

            MakeContextCurrent();

            //XSetErrorHandler(oldX11ErrorHandler);

            return true;
        }

        void Context::SwapBuffers() {
            glXSwapBuffers(m_window->GetX11Display(), m_window->GetNativeWindowHandle());
        }

        void Context::MakeContextNotCurrent() {
            glXMakeCurrent(m_window->GetX11Display(), None, nullptr);
        }

        void Context::MakeContextCurrent() {
            if (!glXMakeCurrent(m_window->GetX11Display(), m_window->GetNativeWindowHandle(), m_rendereringContext)) {
                std::cout << "Failed to make OpenGL context current!\n";
            }
        }


        void* LoadOpenGLFunction(const char* name) {
            void* fun = (void*)glXGetProcAddressARB((const GLubyte*)name);
            return fun;
        }
    }
}
#endif // _WIN32