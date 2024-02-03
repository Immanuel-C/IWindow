#include "IWindowGL.h"
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
#if defined(_WIN32)

#include "IWindowGL.h"
#include <wingdi.h>

typedef HGLRC WINAPI FNP_wglCreateContextAttribsARB(HDC hdc, HGLRC hShareContext,
        const int *attribList);

typedef BOOL WINAPI FNP_wglSwapIntervalEXT(int interval);

typedef BOOL WINAPI FNP_wwglChoosePixelFormatARB(HDC hdc, const int *piAttribIList,
        const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

// See https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_create_context.txt for all values.
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092

#define WGL_CONTEXT_FLAGS_ARB                     0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define WGL_CONTEXT_DEBUG_BIT_ARB                 0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB    0x0002

// See https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt for all values.
#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_DRAW_TO_BITMAP_ARB                    0x2002


#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_NO_ACCELERATION_ARB                   0x2025

#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011

#define WGL_STEREO_ARB                            0x2012
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_ALPHA_BITS_ARB                        0x201B
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023
#define WGL_TYPE_RGBA_ARB                         0x202B

#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB          0x20A9

// See https://developer.download.nvidia.com/opengl/specs/GL_ARB_multisample.txt for all values.
#define WGL_SAMPLE_BUFFERS_ARB		              0x2041
#define WGL_SAMPLES_ARB			                  0x2042

#define WGL_CONTEXT_OPENGL_NO_ERROR_ARB 0x31b3


#define IWINDOW_GL_BACK_LEFT 0x0402
#define IWINDOW_GL_BACK_RIGHT 0x0403

namespace IWindow {
    namespace GL {
        FNP_wglCreateContextAttribsARB* wglCreateContextAttribsARB;
        FNP_wwglChoosePixelFormatARB* wglChoosePixelFormatARB;
        FNP_wglSwapIntervalEXT* wglSwapIntervalEXT;

        // We need to create a dummy context because wgl requires a context before loading
        // any modern wgl functions
        bool CreateDummyAndLoadFunctions(const ContextCreateInfo& contextCreateInfo) {
            WNDCLASS wc{};
            wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            wc.lpfnWndProc = ::DefWindowProc;
            wc.hInstance = ::GetModuleHandleA(nullptr);
            wc.lpszClassName = L"IWindow::DummyWindow";

            if (!::RegisterClass(&wc)) return false;

            HWND dummyWindow = ::CreateWindowEx(
                0,
                wc.lpszClassName,
                L"Dummy OpenGL Window",
                0,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                0,
                0,
                wc.hInstance,
                0
            );

            if (!dummyWindow) return false;

            HDC dummyDeviceContext = GetDC(dummyWindow);

            uint32_t flags = 0;

            flags |= (PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW);
            contextCreateInfo.doubleBuffer ? flags |= PFD_DOUBLEBUFFER : 0;
            contextCreateInfo.steroscopicRendering ? flags |= PFD_STEREO : 0;


            PIXELFORMATDESCRIPTOR pfd{};
            pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
            pfd.nVersion = 1;
            pfd.iPixelType = PFD_TYPE_RGBA;
            pfd.dwFlags = (DWORD)flags;
            pfd.cColorBits = 32;
            pfd.cAlphaBits = 8;
            pfd.iLayerType = PFD_MAIN_PLANE;
            pfd.cDepthBits = 24;
            pfd.cStencilBits = 8;

            int pixelFormat = ::ChoosePixelFormat(dummyDeviceContext, &pfd);

            IWINDOW_CHECK_ERROR(!pixelFormat, ErrorType::OpenGL, ErrorSeverity::FatalError, "ChoosePixelFormat() failed. Failed to find a suitable pixel format!", true, false);

            IWINDOW_CHECK_ERROR(!::SetPixelFormat(dummyDeviceContext, pixelFormat, &pfd), ErrorType::OpenGL, ErrorSeverity::FatalError, "SetPixelFormat() failed. Failed to set a pixel format!", true, false);

            HGLRC dummyRendereringContext = wglCreateContext(dummyDeviceContext);

            IWINDOW_CHECK_ERROR(!dummyDeviceContext, ErrorType::OpenGL, ErrorSeverity::FatalError, "wglCreateContext() failed. Failed to create a dummy OpenGL context!", true, false);

            IWINDOW_CHECK_ERROR(!wglMakeCurrent(dummyDeviceContext, dummyRendereringContext), ErrorType::OpenGL, ErrorSeverity::FatalError, "wglMakeCurrent() failed. Failed to set a dummy OpenGL context!", true, false);

            wglCreateContextAttribsARB = (FNP_wglCreateContextAttribsARB*)Context::LoadOpenGLFunction("wglCreateContextAttribsARB");
            wglChoosePixelFormatARB = (FNP_wwglChoosePixelFormatARB*)Context::LoadOpenGLFunction("wglChoosePixelFormatARB");

            IWINDOW_CHECK_ERROR(!wglCreateContextAttribsARB, ErrorType::OpenGL, ErrorSeverity::FatalError, "Context::LoadOpenGLFunction(\"wglCreateContextAttribsARB\") failed. Failed to load wgl function!", true, false);

            IWINDOW_CHECK_ERROR(!wglChoosePixelFormatARB, ErrorType::OpenGL, ErrorSeverity::FatalError, "Context::LoadOpenGLFunction(\"wglChoosePixelFormatARB\") failed. Failed to load wgl function!", true, false);

            wglMakeCurrent(dummyDeviceContext, nullptr);
            wglDeleteContext(dummyRendereringContext);
            ::ReleaseDC(dummyWindow, dummyDeviceContext);
            ::DestroyWindow(dummyWindow);
            ::UnregisterClass(wc.lpszClassName, nullptr);

            return true;
        }

        Context::Context(Window& window, const ContextCreateInfo& contextCreateInfo) { Create(window, contextCreateInfo); }

        void Context::Destroy() { 
            MakeContextCurrent(false);
            wglDeleteContext(m_rendereringContext);
        }

        bool Context::Create(Window& window, const ContextCreateInfo& contextCreateInfo) {
            m_window = &window;
            
            if (!CreateDummyAndLoadFunctions(contextCreateInfo)) return false;

            // Now we can choose a pixel format the modern way, using wglChoosePixelFormatARB.
            std::array<int, 27> pixelFormatAttribs = {
                WGL_DRAW_TO_WINDOW_ARB,           (int)true,
                WGL_SUPPORT_OPENGL_ARB,           (int)true,
                WGL_DOUBLE_BUFFER_ARB,            (int)contextCreateInfo.doubleBuffer,
                WGL_ACCELERATION_ARB,             WGL_FULL_ACCELERATION_ARB,
                WGL_PIXEL_TYPE_ARB,               WGL_TYPE_RGBA_ARB,
                WGL_STEREO_ARB,                   (int)contextCreateInfo.steroscopicRendering,
                WGL_COLOR_BITS_ARB,               (int)(contextCreateInfo.rgbaBits.r + contextCreateInfo.rgbaBits.g + contextCreateInfo.rgbaBits.b),
                WGL_ALPHA_BITS_ARB,               (int)contextCreateInfo.rgbaBits.a,
                WGL_DEPTH_BITS_ARB,               (int)contextCreateInfo.depthBits,
                WGL_STENCIL_BITS_ARB,             (int)contextCreateInfo.stencilBits,
                WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, (int)contextCreateInfo.sRGB,
                WGL_SAMPLE_BUFFERS_ARB,           (int)(contextCreateInfo.samples > 0),
                WGL_SAMPLES_ARB,                  (int)contextCreateInfo.samples,
                0 // End of array
            };

            int pixelFormat;
            uint32_t numFormats;
            wglChoosePixelFormatARB(window.GetNativeDeviceContext(), pixelFormatAttribs.data(), nullptr, 1, &pixelFormat, &numFormats);

            IWINDOW_CHECK_ERROR(!numFormats, ErrorType::OpenGL, ErrorSeverity::FatalError, "wglChoosePixelFormatARB() failed. Failed to load get available pixel formats!", true, false);

            PIXELFORMATDESCRIPTOR pfd;
            ::DescribePixelFormat(window.GetNativeDeviceContext(), pixelFormat, sizeof(pfd), &pfd);

            IWINDOW_CHECK_ERROR(!::SetPixelFormat(window.GetNativeDeviceContext(), pixelFormat, &pfd), ErrorType::OpenGL, ErrorSeverity::FatalError, "SetPixelFormat() failed. Failed to set the pixel format for the OpenGL context!", true, false);


            int32_t wglProfile = contextCreateInfo.profile == Profile::Core ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB : WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
            int32_t wglContextFlags = 0;
            
            if (contextCreateInfo.debugMode)
                wglContextFlags |= WGL_CONTEXT_DEBUG_BIT_ARB;
            if (contextCreateInfo.forwardCompatibility)
                wglContextFlags |= WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;

            std::array<int, 11> rendereringContextAttribs = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, (int)contextCreateInfo.version.x,
                WGL_CONTEXT_MINOR_VERSION_ARB, (int)contextCreateInfo.version.y,
                WGL_CONTEXT_PROFILE_MASK_ARB,  wglProfile,
                WGL_CONTEXT_FLAGS_ARB, wglContextFlags,
                WGL_CONTEXT_OPENGL_NO_ERROR_ARB, (int)contextCreateInfo.noError,
                0, // end of array
            };

            m_rendereringContext = wglCreateContextAttribsARB(window.GetNativeDeviceContext(), nullptr, rendereringContextAttribs.data());

            IWINDOW_CHECK_ERROR(!m_rendereringContext, ErrorType::OpenGL, ErrorSeverity::FatalError, "wglCreateContextAttribsARB() failed. Failed to get the OpenGL rendering context!", true, false);

            wglMakeCurrent(window.GetNativeDeviceContext(), m_rendereringContext);

            wglSwapIntervalEXT = (FNP_wglSwapIntervalEXT*)LoadOpenGLFunction("wglSwapIntervalEXT");

            IWINDOW_CHECK_ERROR(!wglSwapIntervalEXT, ErrorType::OpenGL, ErrorSeverity::Warning, "LoadOpenGLFunction(\"wglSwapIntervalEXT\") failed. Failed to laod wglSwapIntervalEXT! V-Sync is not supported!", false, false);

            return true;
        }
      

        void Context::SwapFramebuffers() const {
            ::SwapBuffers(m_window->GetNativeDeviceContext());
        }

        void Context::vSync(bool vSync) const {
            if (wglSwapIntervalEXT)
                wglSwapIntervalEXT(vSync);
        }


        void Context::MakeContextCurrent(bool current) const {
            if (current) {
                wglMakeCurrent(m_window->GetNativeDeviceContext(), m_rendereringContext);
                return;
            }

            wglMakeCurrent(m_window->GetNativeDeviceContext(), nullptr);
        }

        void* Context::LoadOpenGLFunction(const char* name) {
            // For opengl functions from version 1.2 to 4.6 (current)
            void* fun = (void*)wglGetProcAddress(name); 
            // While the MSDN documentation says that wglGetProcAddress 
            // returns NULL on failure, some implementations will return other 
            // values. 1, 2, and 3 are used, as well as -1.
            if(fun == nullptr || (fun == (void*)0x1) || (fun == (void*)0x2) || (fun == (void*)0x3) || (fun == (void*)-1) )
            {
                // Old opengl functions opengl 1.1 and below
                HMODULE module = ::LoadLibrary(L"opengl32.dll");
                if (!module) {
                    MessageBoxA(nullptr, "OpenGL32.dll could not be loaded!", "Error", MB_ICONEXCLAMATION | MB_OK);
                    return nullptr;
                }

                fun = (void*)::GetProcAddress(module, name);
            }

            return fun;
        }
    }
}
#endif
