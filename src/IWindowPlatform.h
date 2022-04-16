#pragma once

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef HWND NativeWindowHandle; 
typedef HDC NativeGLDeviceContext;
typedef HGLRC NativeGLRendereringContext;

#endif



