#pragma once


#ifdef _WIN32

#ifdef IWINDOW_BUILD_DLL
#define IWINDOW_API __declspec(dllexport) 
#else
#define IWINDOW_API __declspec(dllimport)
#endif

#endif