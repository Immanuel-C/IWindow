#pragma once

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <iostream>

#include "IWindow.h"

enum IWindowClientApi {
	IWindowClientApi_Unkown,
	IWindowClientApi_OpenGL,
	IWindowClientApi_Vulkan
};

struct ImGui_ImplIWindow_Data {
	IWindow::Window* window;
	IWindow::Window* mouseWindow;
	IWindow::Window::KeyCallback PrevUserCallbackKey;
	IWindow::Window::WindowFocusCallback PrevUserCallbackFocus;
	IWindow::Window::MouseMoveCallback PrevUserCallbackMouseMove;
	IWindow::Window::MouseButtonCallback PrevUserCallbackMouseButton;
	IWindow::Window::MouseScrollCallback PrevUserCallbackScroll;
	IWindow::Window::MouseEnteredCallback PrevUserCallbackMouseEntered;
	IWindow::Window::CharCallback PrevUserCallbackChar;
	// A hook into the WndProc function so we dont remove the WndProc function in IWindow
	WNDPROC IWindowWndProc;

	bool installedCallbacks = false;
	bool callbacksChainForAllWindows = true;
	ImVec2 lastValidMousePos;

};

bool ImGui_ImplIWindow_Init(IWindow::Window& window, bool installCallbacks, IWindowClientApi clientApi);
void ImGui_ImplIWindow_NewFrame();
void ImGui_ImplIWindow_Shutdown();

static void ImGui_ImplIWindow_SetClipboardText(void* userdata, const char* text);
static const char* ImGui_ImplIWindow_GetClipboardText(void* userdata);

void ImGui_ImplIWindow_SetCallbacksChainForAllWindows(bool chainForAllWindows);
bool ImGui_ImplIWindow_ShouldChainCallback(IWindow::Window& window);

void ImGui_ImplIWindow_RestoreCallbacks(IWindow::Window& window);
void ImGui_ImplIWindow_InstallCallbacks(IWindow::Window& window);
