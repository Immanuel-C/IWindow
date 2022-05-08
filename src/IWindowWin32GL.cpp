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

// See https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_create_context.txt for all values
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

typedef BOOL WINAPI FNP_wwglChoosePixelFormatARB(HDC hdc, const int *piAttribIList,
        const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

// See https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt for all values
#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023

#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B

namespace IWindow {
    namespace GL {
        FNP_wglCreateContextAttribsARB* wglCreateContextAttribsARB;
        FNP_wwglChoosePixelFormatARB* wglChoosePixelFormatARB;

        // We need to create a dummy context because wgl requires a context before loading
        // any modern wgl functions
        bool CreateDummyAndLoadFunctions() {
            WNDCLASS wc{};
            wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            wc.lpfnWndProc = ::DefWindowProc;
            wc.hInstance = ::GetModuleHandleA(nullptr);
            wc.lpszClassName = L"IWindow::DummyWindow";

            if (!RegisterClass(&wc)) return false;

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

            PIXELFORMATDESCRIPTOR pfd{};
            pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
            pfd.nVersion = 1;
            pfd.iPixelType = PFD_TYPE_RGBA;
            pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            pfd.cColorBits = 32;
            pfd.cAlphaBits = 8;
            pfd.iLayerType = PFD_MAIN_PLANE;
            pfd.cDepthBits = 24;
            pfd.cStencilBits = 8;

            int pixelFormat = ::ChoosePixelFormat(dummyDeviceContext, &pfd);

            if (!pixelFormat) return false;

            if (!::SetPixelFormat(dummyDeviceContext, pixelFormat, &pfd)) return false;

            HGLRC dummyRendereringContext = wglCreateContext(dummyDeviceContext);

            if (!dummyRendereringContext) return false;

            if (!wglMakeCurrent(dummyDeviceContext, dummyRendereringContext)) return false;

            wglCreateContextAttribsARB = (FNP_wglCreateContextAttribsARB*)LoadOpenGLFunction("wglCreateContextAttribsARB");
            wglChoosePixelFormatARB = (FNP_wwglChoosePixelFormatARB*)LoadOpenGLFunction("wglChoosePixelFormatARB");

            if (!wglCreateContextAttribsARB || !wglChoosePixelFormatARB) return false;

            wglMakeCurrent(dummyDeviceContext, nullptr);
            wglDeleteContext(dummyRendereringContext);
            ::ReleaseDC(dummyWindow, dummyDeviceContext);
            ::DestroyWindow(dummyWindow);

            return true;
        }

        Context::Context(Window& window, uint16_t majorVersion, uint16_t minorVersion) : m_window { &window } { Create(window, majorVersion, minorVersion); }

        Context::~Context() { 
            MakeContextNotCurrent();
            wglDeleteContext(m_rendereringContext);
        }

        bool Context::Create(Window& window, uint16_t majorVersion, uint16_t minorVersion) {
            m_window = &window;
            
            if (!CreateDummyAndLoadFunctions()) return false;

            // Now we can choose a pixel format the modern way, using wglChoosePixelFormatARB.
            std::array<int, 17> pixelFormatAttribs = {
                WGL_DRAW_TO_WINDOW_ARB,     true,
                WGL_SUPPORT_OPENGL_ARB,     true,
                WGL_DOUBLE_BUFFER_ARB,      true,
                WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
                WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
                WGL_COLOR_BITS_ARB,         32,
                WGL_DEPTH_BITS_ARB,         24,
                WGL_STENCIL_BITS_ARB,       8,
                0 // End of array
            };

            int pixelFormat;
            uint32_t numFormats;
            wglChoosePixelFormatARB(window.GetNativeDeviceContext(), pixelFormatAttribs.data(), nullptr, 1, &pixelFormat, &numFormats);

            if (!numFormats) return false;

            PIXELFORMATDESCRIPTOR pfd;
            DescribePixelFormat(window.GetNativeDeviceContext(), pixelFormat, sizeof(pfd), &pfd);
            if (!SetPixelFormat(window.GetNativeDeviceContext(), pixelFormat, &pfd)) return false;

            std::array<int, 7>rendereringContextAttribs = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, majorVersion,
                WGL_CONTEXT_MINOR_VERSION_ARB, minorVersion,
                WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                0, // end of array
            };

            m_rendereringContext = wglCreateContextAttribsARB(window.GetNativeDeviceContext(), nullptr, rendereringContextAttribs.data());

            if (!m_rendereringContext) return false;

            if (!wglMakeCurrent(window.GetNativeDeviceContext(), m_rendereringContext)) return false;

            return true;
        }
      

        void Context::SwapBuffers() { 
            ::SwapBuffers(m_window->GetNativeDeviceContext());
        }

        void Context::MakeContextNotCurrent() {
            wglMakeCurrent(m_window->GetNativeDeviceContext(), nullptr);
        }

        void Context::MakeContextCurrent()
        {
            wglMakeCurrent(m_window->GetNativeDeviceContext(), m_rendereringContext);
        }

        void* LoadOpenGLFunction(const char* name) {
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