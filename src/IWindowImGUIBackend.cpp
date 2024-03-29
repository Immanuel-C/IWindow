/*BSD 2 - Clause License

Copyright(c) 2022, Immanuel Charles
All rights reserved.

Redistributionand use in sourceand binary forms, with or without
modification, are permitted provided that the following conditions are met :

1. Redistributions of source code must retain the above copyright notice, this
list of conditionsand the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditionsand the following disclaimer in the documentation
and /or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "IWindowImGUIBackend.h"

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
	IWindow::Window::MonitorCallback PrevUserCallbackMonitor;
	// A hook into the WndProc function so we dont remove the WndProc function in IWindow
	WNDPROC IWindowWndProc;

	std::array<IWindow::CursorID, (size_t)IWindow::CursorID::Max> mouseCursors;

	bool installedCallbacks = false;
	bool callbacksChainForAllWindows = true;
	bool wantUpdateMonitors = true;
	ImVec2 lastValidMousePos;

	double time;
};

static ImGuiMouseSource GetMouseSourceFromMessageExtraInfo() {
	LPARAM extra_info = ::GetMessageExtraInfo();
	if ((extra_info & 0xFFFFFF80) == 0xFF515700)
		return ImGuiMouseSource_Pen;
	if ((extra_info & 0xFFFFFF80) == 0xFF515780)
		return ImGuiMouseSource_TouchScreen;
	return ImGuiMouseSource_Mouse;
}

ImGui_ImplIWindow_Data* ImGui_ImplIWindow_GetBackendData() {
	return ImGui::GetCurrentContext() ? (ImGui_ImplIWindow_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
}

LRESULT CALLBACK ImGui_ImplIWindow_WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	switch (msg)
	{
	case WM_MOUSEMOVE: case WM_NCMOUSEMOVE:
	case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK: case WM_LBUTTONUP:
	case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK: case WM_RBUTTONUP:
	case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK: case WM_MBUTTONUP:
	case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK: case WM_XBUTTONUP:
		ImGui::GetIO().AddMouseSourceEvent(GetMouseSourceFromMessageExtraInfo());
		break;
	}
	return ::CallWindowProc(data->IWindowWndProc, hWnd, msg, wparam, lparam);
}


int ImGui_ImplIWindow_ButtonToImGuiButton(IWindow::MouseButton button) {
	switch (button)
	{
	case IWindow::MouseButton::Left:
		return 0;
	case IWindow::MouseButton::Right:
		return 1;
	case IWindow::MouseButton::Middle:
		return 2;
	default:
		return -1;
	}
}

ImGuiKey ImGui_ImplIWindow_KeyToImGuiKey(IWindow::Key key) {
	switch (key)
	{
	case IWindow::Key::N0:
		return ImGuiKey_0;
	case IWindow::Key::N1:
		return ImGuiKey_1;
	case IWindow::Key::N2:
		return ImGuiKey_2;
	case IWindow::Key::N3:
		return ImGuiKey_3;
	case IWindow::Key::N4:
		return ImGuiKey_4;
	case IWindow::Key::N5:
		return ImGuiKey_5;
	case IWindow::Key::N6:
		return ImGuiKey_6;
	case IWindow::Key::N7:
		return ImGuiKey_7;
	case IWindow::Key::N8:
		return ImGuiKey_8;
	case IWindow::Key::N9:
		return ImGuiKey_9;
	case IWindow::Key::A:
		return ImGuiKey_A;
	case IWindow::Key::B:
		return ImGuiKey_B;
	case IWindow::Key::C:
		return ImGuiKey_C;
	case IWindow::Key::D:
		return ImGuiKey_D;
	case IWindow::Key::E:
		return ImGuiKey_E;
	case IWindow::Key::F:
		return ImGuiKey_F;
	case IWindow::Key::G:
		return ImGuiKey_G;
	case IWindow::Key::H:
		return ImGuiKey_H;
	case IWindow::Key::I:
		return ImGuiKey_I;
	case IWindow::Key::J:
		return ImGuiKey_J;
	case IWindow::Key::K:
		return ImGuiKey_K;
	case IWindow::Key::L:
		return ImGuiKey_L;
	case IWindow::Key::M:
		return ImGuiKey_M;
	case IWindow::Key::N:
		return ImGuiKey_N;
	case IWindow::Key::O:
		return ImGuiKey_O;
	case IWindow::Key::P:
		return ImGuiKey_P;
	case IWindow::Key::Q:
		return ImGuiKey_Q;
	case IWindow::Key::R:
		return ImGuiKey_R;
	case IWindow::Key::S:
		return ImGuiKey_S;
	case IWindow::Key::T:
		return ImGuiKey_T;
	case IWindow::Key::U:
		return ImGuiKey_U;
	case IWindow::Key::V:
		return ImGuiKey_V;
	case IWindow::Key::W:
		return ImGuiKey_W;
	case IWindow::Key::X:
		return ImGuiKey_X;
	case IWindow::Key::Y:
		return ImGuiKey_Y;
	case IWindow::Key::Z:
		return ImGuiKey_Z;
	case IWindow::Key::Back:
		return ImGuiKey_Backspace;
	case IWindow::Key::Tab:
		return ImGuiKey_Tab;
	case IWindow::Key::Enter:
		return ImGuiKey_Enter;
	case IWindow::Key::Pause:
		return ImGuiKey_Pause;
	case IWindow::Key::Escape:
		return ImGuiKey_Escape;
	case IWindow::Key::Space:
		return ImGuiKey_Space;
	case IWindow::Key::PageUp:
		return ImGuiKey_PageUp;
	case IWindow::Key::PageDown:
		return ImGuiKey_PageDown;
	case IWindow::Key::End:
		return ImGuiKey_End;
	case IWindow::Key::Home:
		return ImGuiKey_Home;
	case IWindow::Key::ArrowLeft:
		return ImGuiKey_LeftArrow;
	case IWindow::Key::ArrowUp:
		return ImGuiKey_UpArrow;
	case IWindow::Key::ArrowRight:
		return ImGuiKey_RightArrow;
	case IWindow::Key::ArrowDown:
		return ImGuiKey_DownArrow;
	case IWindow::Key::PrintScreen:
		return ImGuiKey_PrintScreen;
	case IWindow::Key::Insert:
		return ImGuiKey_Insert;
	case IWindow::Key::Delete:
		return ImGuiKey_Delete;
	case IWindow::Key::Add:
		return ImGuiKey_KeypadAdd;
	case IWindow::Key::Multiply:
		return ImGuiKey_KeypadMultiply;
	case IWindow::Key::Subtract:
		return ImGuiKey_Minus;
	case IWindow::Key::Decimal:
		return ImGuiKey_Period;
	case IWindow::Key::Divide:
		return ImGuiKey_KeypadDivide;
	case IWindow::Key::ScrollLock:
		return ImGuiKey_None;
	case IWindow::Key::Numpad0:
		return ImGuiKey_Keypad0;
	case IWindow::Key::Numpad1:
		return ImGuiKey_Keypad1;
	case IWindow::Key::Numpad2:
		return ImGuiKey_Keypad2;
	case IWindow::Key::Numpad3:
		return ImGuiKey_Keypad3;
	case IWindow::Key::Numpad4:
		return ImGuiKey_Keypad4;
	case IWindow::Key::Numpad5:
		return ImGuiKey_Keypad5;
	case IWindow::Key::Numpad6:
		return ImGuiKey_Keypad6;
	case IWindow::Key::Numpad7:
		return ImGuiKey_Keypad7;
	case IWindow::Key::Numpad8:
		return ImGuiKey_Keypad8;
	case IWindow::Key::Numpad9:
		return ImGuiKey_Keypad9;
	case IWindow::Key::F1:
		return ImGuiKey_F1;
	case IWindow::Key::F2:
		return ImGuiKey_F2;
	case IWindow::Key::F3:
		return ImGuiKey_F3;
	case IWindow::Key::F4:
		return ImGuiKey_F4;
	case IWindow::Key::F5:
		return ImGuiKey_F5;
	case IWindow::Key::F6:
		return ImGuiKey_F6;
	case IWindow::Key::F7:
		return ImGuiKey_F7;
	case IWindow::Key::F8:
		return ImGuiKey_F8;
	case IWindow::Key::F9:
		return ImGuiKey_F9;
	case IWindow::Key::F10:
		return ImGuiKey_F10;
	case IWindow::Key::F11:
		return ImGuiKey_F11;
	case IWindow::Key::F12:
		return ImGuiKey_F12;
	case IWindow::Key::SemiColen:
		return ImGuiKey_Semicolon;
	case IWindow::Key::Plus:
		return ImGuiKey_KeypadAdd;
	case IWindow::Key::Comma:
		return ImGuiKey_Comma;
	case IWindow::Key::Minus:
		return ImGuiKey_Minus;
	case IWindow::Key::Period:
		return ImGuiKey_Period;
	case IWindow::Key::ForwardSlash:
		return ImGuiKey_Slash;
	case IWindow::Key::Tilde:
		return ImGuiKey_None;
	case IWindow::Key::LeftBoxBraces:
		return ImGuiKey_None;
	case IWindow::Key::BackSlash:
		return ImGuiKey_Slash;
	case IWindow::Key::RightBoxBraces:
		return ImGuiKey_None;
	case IWindow::Key::SingleQuotes:
		return ImGuiKey_None;
	case IWindow::Key::Max:
		return ImGuiKey_None;
	case IWindow::Key::LControl:
		return ImGuiKey_LeftCtrl;
	case IWindow::Key::RControl:
		return ImGuiKey_RightCtrl;
	case IWindow::Key::LAlt:
		return ImGuiKey_LeftAlt;
	case IWindow::Key::RAlt:
		return ImGuiKey_RightAlt;
	case IWindow::Key::LShift:
		return ImGuiKey_LeftShift;
	case IWindow::Key::RShift:
		return ImGuiKey_RightShift;
	case IWindow::Key::LSuper:
		return ImGuiKey_LeftSuper;
	case IWindow::Key::RSuper:
		return ImGuiKey_RightSuper;
	case IWindow::Key::CapsLock:
		return ImGuiKey_CapsLock;
	case IWindow::Key::NumLock:
		return ImGuiKey_NumLock;
	default:
		return ImGuiKey_None;
	}
}


static void ImGui_ImplIWindow_SetClipboardText(void* userdata, const char* text) {
	IWindow::Window* window = (IWindow::Window*)userdata;
	window->SetClipboardText(text);
}

static const char* ImGui_ImplIWindow_GetClipboardText(void* userdata) {
	IWindow::Window* window = (IWindow::Window*)userdata;
	const char* text = _strdup(window->GetClipboardText().c_str());
	return text;
}

bool ImGui_ImplIWindow_Init(IWindow::Window& window, bool installCallbacks) {
	ImGuiIO& io = ImGui::GetIO();
	if (io.BackendPlatformUserData != nullptr) {
		std::cout << "Already initialized a platform backend!\n";
		return false;
	}

	ImGui_ImplIWindow_Data* data = IM_NEW(ImGui_ImplIWindow_Data)();
	io.BackendPlatformUserData = (void*)data;
	io.BackendPlatformName = "imgui_impl_iwindow";
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	data->window = &window;

	io.SetClipboardTextFn = ImGui_ImplIWindow_SetClipboardText;
	io.GetClipboardTextFn = ImGui_ImplIWindow_GetClipboardText;
	io.UserData = &window;

	data->mouseCursors[ImGuiMouseCursor_Arrow] = IWindow::CursorID::Arrow;
	data->mouseCursors[ImGuiMouseCursor_TextInput] = IWindow::CursorID::IBeam;
	data->mouseCursors[ImGuiMouseCursor_ResizeNS] = IWindow::CursorID::VerticalResize;
	data->mouseCursors[ImGuiMouseCursor_ResizeEW] = IWindow::CursorID::HorizontalResize;
	data->mouseCursors[ImGuiMouseCursor_Hand] = IWindow::CursorID::Hand;
	data->mouseCursors[ImGuiMouseCursor_ResizeAll] = IWindow::CursorID::Move;
	data->mouseCursors[ImGuiMouseCursor_ResizeNWSE] = IWindow::CursorID::DiagonalResize1;
	data->mouseCursors[ImGuiMouseCursor_ResizeNESW] = IWindow::CursorID::DiagonalResize2;
	data->mouseCursors[ImGuiMouseCursor_NotAllowed] = IWindow::CursorID::No;

	if (installCallbacks)
		ImGui_ImplIWindow_InstallCallbacks(window);

	ImGuiViewport* mainViewport = ImGui::GetMainViewport();
	mainViewport->PlatformHandleRaw = window.GetNativeWindowHandle();

	data->IWindowWndProc = (WNDPROC)::GetWindowLongPtr((HWND)mainViewport->PlatformHandleRaw, GWLP_WNDPROC);
	if (data->IWindowWndProc == nullptr) {
		std::cout << "ImGui_Impl_IWindow: Failed to register WndProc hook. Failed to init ImGui_ImplIWindow\n";
		return false;
	}
	::SetWindowLongPtr((HWND)mainViewport->PlatformHandleRaw, GWLP_WNDPROC, (LONG_PTR)ImGui_ImplIWindow_WndProc);

	return true;
}

void ImGui_ImplIWindow_UpdateMouseData() {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	ImGuiIO& io = ImGui::GetIO();

	IWindow::Window* window = data->window;

	if (window->IsFocused()) 
		if (io.WantSetMousePos)
			window->SetMousePosition({ (int32_t)io.MousePos.x, (int32_t)io.MousePos.y });
}

static void ImGui_ImplIWindow_UpdateMouseCursor() {
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	IWindow::Window* window = data->window;

	ImGuiMouseCursor imGuiCursor = ImGui::GetMouseCursor();
	if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange))
		return;

	// (those braces are here to reduce diff with multi-viewports support in 'docking' branch)
	{
		if (imGuiCursor == ImGuiMouseCursor_None || io.MouseDrawCursor) 
			// Hide cursor if imgui wants to draw to it or if it wants no cursor
			window->SetCursor(IWindow::CursorID::Hidden);
		else {
			window->SetCursor(data->mouseCursors[imGuiCursor]);
		}
	}
}

void ImGui_ImplIWindow_NewFrame() { 
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	if (data == nullptr) {
		std::cerr << "Did you call ImGui_ImplIWindow_Init?\n";
		return;
	}
	IWindow::Vector2<int32_t> windowSize, frameBufferSize;
	windowSize = data->window->GetWindowSize();
	frameBufferSize = data->window->GetFramebufferSize();
	io.DisplaySize = ImVec2((float)windowSize.x, (float)windowSize.y);
	// window not minimized
	// Causes Imgui to think the mouse is offset and program works fine without
	if (windowSize.x > 0 && windowSize.y > 0)
		io.DisplayFramebufferScale = ImVec2(((float)frameBufferSize.x / (float)windowSize.x), ((float)frameBufferSize.y / (float)windowSize.y));


	// Get delta time
	double currentTime = data->window->GetTime();
	// ?
	if (currentTime <= data->time)
		currentTime = data->time + 0.00001f;
	io.DeltaTime = data->time > 0.0 ? (float)(currentTime - data->time) : (float)(1.0f / 60.0f);
	data->time = currentTime;

	
	ImGui_ImplIWindow_UpdateMouseData();
	// Broken
	// ImGui_ImplIWindow_UpdateMouseCursor();
}

void ImGui_ImplIWindow_Shutdown() { 
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	if (data == nullptr) {
		std::cout << "No platform backend to shutdown, or already shutdown?\n";
		return;
	}
	ImGuiIO& io = ImGui::GetIO();
	if (data->installedCallbacks)
		ImGui_ImplIWindow_RestoreCallbacks(*data->window);

	ImGuiViewport* mainViewport = ImGui::GetMainViewport();
	::SetWindowLongPtr((HWND)mainViewport->PlatformHandleRaw, GWLP_WNDPROC, (LONG_PTR)data->IWindowWndProc);
	data->IWindowWndProc = nullptr;

	io.BackendPlatformName = nullptr;
	io.BackendPlatformUserData = nullptr;
	io.BackendFlags &= ~(ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_HasSetMousePos);

	IM_DELETE(data);
}

void ImGui_ImplIWindow_SetCallbacksChainForAllWindows(bool chainForAllWindows)
{
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	data->callbacksChainForAllWindows = chainForAllWindows;
}

// ?
bool ImGui_ImplIWindow_ShouldChainCallback(IWindow::Window& window)
{
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();

	return data->callbacksChainForAllWindows ? true : (window == (*data->window));
}


static void ImGui_ImplIWindow_UpdateKeyModifiers(IWindow::Window& window, ImGuiIO& io) {
	io.AddKeyEvent(ImGuiMod_Ctrl, window.IsKeyModifiersDown(IWindow::KeyModifier::Control));
	io.AddKeyEvent(ImGuiMod_Shift, window.IsKeyModifiersDown(IWindow::KeyModifier::Shift));
	io.AddKeyEvent(ImGuiMod_Alt, window.IsKeyModifiersDown(IWindow::KeyModifier::Alt));
	io.AddKeyEvent(ImGuiMod_Super, window.IsKeyModifiersDown(IWindow::KeyModifier::Super));
}

void ImGui_ImplIWindow_MouseButtonCallback(IWindow::Window& window, IWindow::MouseButton button, IWindow::KeyModifier mods, IWindow::InputState state) {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	if (data->PrevUserCallbackMouseButton != nullptr && ImGui_ImplIWindow_ShouldChainCallback(window))
		data->PrevUserCallbackMouseButton(window, button, mods, state);

	ImGuiIO& io = ImGui::GetIO();

	int translatedButton = ImGui_ImplIWindow_ButtonToImGuiButton(button);

	ImGui_ImplIWindow_UpdateKeyModifiers(window, io);

	if (translatedButton >= 0 && translatedButton < ImGuiMouseButton_COUNT)
		io.AddMouseButtonEvent(translatedButton, state == IWindow::InputState::Down);
}


void ImGui_ImplIWindow_ScrollCallback(IWindow::Window& window, IWindow::Vector2<float> scrollOffset) {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	if (data->PrevUserCallbackScroll != nullptr && ImGui_ImplIWindow_ShouldChainCallback(window))
		data->PrevUserCallbackScroll(window, scrollOffset);

	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseWheelEvent(scrollOffset.x, scrollOffset.y);
}

void ImGui_ImplIWindow_KeyCallback(IWindow::Window& window, IWindow::Key key, IWindow::KeyModifier mods, IWindow::InputState state, bool repeated) {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	if (data->PrevUserCallbackKey != nullptr && ImGui_ImplIWindow_ShouldChainCallback(window))
		data->PrevUserCallbackKey(window, key, mods, state, repeated);

	ImGuiKey translatedKey = ImGui_ImplIWindow_KeyToImGuiKey(key);
	if (translatedKey == ImGuiKey_None)
		return;

	if (repeated && state == IWindow::InputState::Down)
		return;

	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplIWindow_UpdateKeyModifiers(window, io);
	io.AddKeyEvent(translatedKey, state == IWindow::InputState::Down);
}

void ImGui_ImplIWindow_WindowFocusCallback(IWindow::Window& window, bool focused) {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	if (data->PrevUserCallbackFocus != nullptr && ImGui_ImplIWindow_ShouldChainCallback(window))
		data->PrevUserCallbackFocus(window, focused);

	ImGuiIO& io = ImGui::GetIO();
	io.AddFocusEvent(focused == true);
}


void ImGui_ImplIWindow_MousePosCallback(IWindow::Window& window, IWindow::Vector2<int32_t> mousePosition) {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	if (data->PrevUserCallbackMouseMove != nullptr && ImGui_ImplIWindow_ShouldChainCallback(window)) data->PrevUserCallbackMouseMove(window, mousePosition);

	ImGuiIO& io = ImGui::GetIO();
	io.AddMousePosEvent((float)mousePosition.x, (float)mousePosition.y);
	data->lastValidMousePos = ImVec2((float)mousePosition.x, (float)mousePosition.y);
}

void ImGui_ImplIWindow_CursorEnterCallback(IWindow::Window& window, bool entered) {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	if (data->PrevUserCallbackMouseEntered != nullptr && ImGui_ImplIWindow_ShouldChainCallback(window)) data->PrevUserCallbackMouseEntered(window, entered);

	ImGuiIO& io = ImGui::GetIO();
	if (entered) {
		data->mouseWindow = &window;
		io.AddMousePosEvent(data->lastValidMousePos.x, data->lastValidMousePos.y);
	}
	else if (!entered && data->mouseWindow == &window) {
		data->lastValidMousePos = io.MousePos;
		data->mouseWindow = nullptr;
		// Im guessing the negative float max tells imgui that the mouse is not on the window
		io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
	}
}

void ImGui_ImplIWindow_CharCallback(IWindow::Window& window, char32_t c, IWindow::KeyModifier mods) {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	if (data->PrevUserCallbackChar != nullptr && ImGui_ImplIWindow_ShouldChainCallback(window))
		data->PrevUserCallbackChar(window, c, mods);


	ImGuiIO& io = ImGui::GetIO();
	io.AddInputCharacter(c);
}

void ImGui_ImplIWindow_MonitorCallback(IWindow::Window&, const IWindow::Monitor&, bool) {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	data->wantUpdateMonitors = true;
}


void ImGui_ImplIWindow_RestoreCallbacks(IWindow::Window& window) {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	if (data->installedCallbacks == false) {
		std::cout << "ImGui_Impl_IWindow: Callbacks not installed!\n";
		return;
	}
	if ((*data->window) != window) {
		std::cout << "ImGui_Impl_IWindow: cant restore callbacks: Wrong window!\n";
		return;
	}

	window.SetWindowFocusCallback(data->PrevUserCallbackFocus);
	window.SetMouseEnteredCallback(data->PrevUserCallbackMouseEntered);
	window.SetMouseMoveCallback(data->PrevUserCallbackMouseMove);
	window.SetMouseButtonCallback(data->PrevUserCallbackMouseButton);
	window.SetMouseScrollCallback(data->PrevUserCallbackScroll);
	window.SetKeyCallback(data->PrevUserCallbackKey);
	window.SetCharCallback(data->PrevUserCallbackChar);
	window.SetMonitorCallback(data->PrevUserCallbackMonitor);
	data->PrevUserCallbackFocus = nullptr;
	data->PrevUserCallbackMouseEntered = nullptr;
	data->PrevUserCallbackMouseMove = nullptr;
	data->PrevUserCallbackMouseButton = nullptr;
	data->PrevUserCallbackScroll = nullptr;
	data->PrevUserCallbackKey = nullptr;
	data->PrevUserCallbackChar = nullptr;
	data->PrevUserCallbackMonitor = nullptr;
	data->installedCallbacks = false;

}

void ImGui_ImplIWindow_InstallCallbacks(IWindow::Window& window) {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	if (data->installedCallbacks) {
		std::cout << "ImGui_Impl_IWindow: Callbacks already installed. Did you call ImGui_ImplIWindow_Init twice?\n";
	}
	
	data->PrevUserCallbackFocus = window.SetWindowFocusCallback(ImGui_ImplIWindow_WindowFocusCallback);
	data->PrevUserCallbackMouseEntered = window.SetMouseEnteredCallback(ImGui_ImplIWindow_CursorEnterCallback);
	data->PrevUserCallbackMouseMove = window.SetMouseMoveCallback(ImGui_ImplIWindow_MousePosCallback);
	data->PrevUserCallbackMouseButton = window.SetMouseButtonCallback(ImGui_ImplIWindow_MouseButtonCallback);
	data->PrevUserCallbackScroll = window.SetMouseScrollCallback(ImGui_ImplIWindow_ScrollCallback);
	data->PrevUserCallbackKey = window.SetKeyCallback(ImGui_ImplIWindow_KeyCallback);
	data->PrevUserCallbackChar = window.SetCharCallback(ImGui_ImplIWindow_CharCallback);
	data->PrevUserCallbackMonitor = window.SetMonitorCallback(ImGui_ImplIWindow_MonitorCallback);
	data->installedCallbacks = true;
}
