#pragma once


#ifdef _WIN32
#ifdef IWINDOW_DLL
#ifdef IWINDOW_BUILD_DLL
#define IWINDOW_API __declspec(dllexport) 
#else
#define IWINDOW_API __declspec(dllimport)
#endif
#else
#define IWINDOW_API
#endif
#endif


#if defined(DEBUG) || defined(IWINDOW_ALWAYS_CHECK_ERRORS)
#define IWINDOW_CHECK_ERROR(result, nType, nSeverity, nMessage, shouldReturn, returnVal)		\
			if (result) {																				\
				IWindow::Error err{};																			\
				err.type = nType;																				\
				err.severity = nSeverity;																		\
				err.message = nMessage;																			\
																												\
				IWindow::GetErrorCallback()(err);													\
				if (shouldReturn) return returnVal;																\
			}
#else
#define IWINDOW_CHECK_ERROR(result, badVal, type, severity, source, message, shouldReturn, returnVal) result;
#endif