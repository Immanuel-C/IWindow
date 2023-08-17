//BSD 2 - Clause License
//
//Copyright(c) 2022, Immanuel Charles
//All rights reserved.
//
//Redistributionand use in sourceand binary forms, with or without
//modification, are permitted provided that the following conditions are met :
//
//1. Redistributions of source code must retain the above copyright notice, this
//list of conditionsand the following disclaimer.
//
//2. Redistributions in binary form must reproduce the above copyright notice,
//this list of conditionsand the following disclaimer in the documentation
//and /or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//	OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#pragma once

#include <imgui.h>
#include <iostream>

#include "IWindow.h"


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

	double time;

};

bool ImGui_ImplIWindow_Init(IWindow::Window& window, bool installCallbacks = true);
void ImGui_ImplIWindow_NewFrame();
void ImGui_ImplIWindow_Shutdown();

static void ImGui_ImplIWindow_SetClipboardText(void* userdata, const char* text);
static const char* ImGui_ImplIWindow_GetClipboardText(void* userdata);

void ImGui_ImplIWindow_SetCallbacksChainForAllWindows(bool chainForAllWindows);
bool ImGui_ImplIWindow_ShouldChainCallback(IWindow::Window& window);

void ImGui_ImplIWindow_RestoreCallbacks(IWindow::Window& window);
void ImGui_ImplIWindow_InstallCallbacks(IWindow::Window& window);
