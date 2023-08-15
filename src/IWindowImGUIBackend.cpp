#include "IWindowImGUIBackend.h"

static ImGuiMouseSource GetMouseSourceFromMessageExtraInfo() {
	LPARAM extra_info = ::GetMessageExtraInfo();
	if ((extra_info & 0xFFFFFF80) == 0xFF515700)
		return ImGuiMouseSource_Pen;
	if ((extra_info & 0xFFFFFF80) == 0xFF515780)
		return ImGuiMouseSource_TouchScreen;
	return ImGuiMouseSource_Mouse;
}

ImGui_ImplIWindow_Data* ImGui_ImplIWindow_GetBackendData() {
	return ImGui::GetCurrentContext() ? (ImGui_ImplIWindow_Data*)ImGui::GetIO().BackendLanguageUserData : nullptr;
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
		return 1;
	case IWindow::MouseButton::Right:
		return 2;
	case IWindow::MouseButton::Middle:
		return 3;
	case IWindow::MouseButton::Side1:
		return 4;
	case IWindow::MouseButton::Side2:
		return 5;
	case IWindow::MouseButton::Max:
		return 0;
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
	case IWindow::Key::Alt:
		return ImGuiKey_LeftAlt;
	case IWindow::Key::Pause:
		return ImGuiKey_Pause;
	case IWindow::Key::CapsLock:
		return ImGuiKey_CapsLock;
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
	case IWindow::Key::LeftSuper:
		return ImGuiKey_LeftSuper;
	case IWindow::Key::RightSuper:
		return ImGuiKey_RightSuper;
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
		return ImGuiKey_ScrollLock;
	case IWindow::Key::NumLock:
		return ImGuiKey_NumLock;
	case IWindow::Key::LShift:
		return ImGuiKey_LeftShift;
	case IWindow::Key::RShift:
		return ImGuiKey_RightShift;
	case IWindow::Key::LControl:
		return ImGuiKey_LeftCtrl;
	case IWindow::Key::RControl:
		return ImGuiKey_RightCtrl;
	case IWindow::Key::LMenu:
		return ImGuiKey_Menu;
	case IWindow::Key::RMenu:
		return ImGuiKey_Menu;
	case IWindow::Key::Np0:
		return ImGuiKey_Keypad0;
	case IWindow::Key::Np1:
		return ImGuiKey_Keypad1;
	case IWindow::Key::Np2:
		return ImGuiKey_Keypad2;
	case IWindow::Key::Np3:
		return ImGuiKey_Keypad3;
	case IWindow::Key::Np4:
		return ImGuiKey_Keypad4;
	case IWindow::Key::Np5:
		return ImGuiKey_Keypad5;
	case IWindow::Key::Np6:
		return ImGuiKey_Keypad6;
	case IWindow::Key::Np7:
		return ImGuiKey_Keypad7;
	case IWindow::Key::Np8:
		return ImGuiKey_Keypad8;
	case IWindow::Key::Np9:
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
	case IWindow::Key::Max:
		return -1;
	}
}


bool ImGui_ImplIWindow_Init(IWindow::Window& window, bool installCallbacks, IWindowClientApi clientApi) {
	ImGuiIO io = ImGui::GetIO();
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

	if (installCallbacks)
		ImGui_ImplIWindow_InstallCallbacks(window);

	ImGuiViewport* mainViewport = ImGui::GetMainViewport();
	mainViewport->PlatformHandleRaw = window.GetNativeWindowHandle();

	data->IWindowWndProc = (WNDPROC)::GetWindowLongPtr((HWND)mainViewport->PlatformHandleRaw, GWLP_WNDPROC);
	if (data->IWindowWndProc == nullptr) {
		std::cout << "ImGui_Impl_IWindow: Failed to register WndProc hook. Failed to init ImGui_ImplIWindow\n";
		return;
	}
	::SetWindowLongPtr((HWND)mainViewport->PlatformHandleRaw, GWLP_WNDPROC, (LONG_PTR)ImGui_ImplIWindow_WndProc);
}

void ImGui_ImplIWindow_NewFrame() { ImGui_ImplWin32_NewFrame(); }

void ImGui_ImplIWindow_Shutdown() { ImGui_ImplWin32_Shutdown(); }

void ImGui_ImplIWindow_SetClipboardText(void* userdata, const char* text) {
	IWindow::Window* window = (IWindow::Window*)userdata;
	window->SetClipboardText(text);
}

const char* ImGui_ImplIWindow_GetClipboardText(void* userdata) {
	IWindow::Window* window = (IWindow::Window*)userdata;
	const char* text = strdup(window->GetClipboardText().c_str());
	return text;
	
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

void ImGui_ImplIWindow_MouseButtonCallback(IWindow::Window& window, IWindow::MouseButton button, IWindow::InputState state) {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	if (data->PrevUserCallbackMouseButton != nullptr && ImGui_ImplIWindow_ShouldChainCallback(window))
		data->PrevUserCallbackMouseButton(window, button, state);

	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseButtonEvent(ImGui_ImplIWindow_ButtonToImGuiButton(button), state == IWindow::InputState::Down);
}


void ImGui_ImplIWindow_ScrollCallback(IWindow::Window& window, float xOffset, float yOffset) {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	if (data->PrevUserCallbackScroll != nullptr && ImGui_ImplIWindow_ShouldChainCallback(window))
		data->PrevUserCallbackScroll(window, xOffset, yOffset);

	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseWheelEvent(xOffset, yOffset);
}


void ImGui_ImplIWindow_KeyCallback(IWindow::Window& window, IWindow::Key key, IWindow::InputState state) {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	if (data->PrevUserCallbackKey != nullptr && ImGui_ImplIWindow_ShouldChainCallback(window))
		data->PrevUserCallbackKey(window, key, state);

	ImGuiKey translatedKey = ImGui_ImplIWindow_KeyToImGuiKey(key);
	ImGuiIO& io = ImGui::GetIO();
	io.AddKeyEvent(translatedKey, state == IWindow::InputState::Down);
}

void ImGui_ImplIWindow_WindowFocusCallback(IWindow::Window& window, bool focused) {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	if (data->PrevUserCallbackFocus != nullptr && ImGui_ImplIWindow_ShouldChainCallback(window))
		data->PrevUserCallbackFocus(window, focused);

	ImGuiIO& io = ImGui::GetIO();
	io.AddFocusEvent(focused == true);
}


void ImGui_ImplIWindow_MousePosCallback(IWindow::Window& window, int64_t x, int64_t y) {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	if (data->PrevUserCallbackMouseMove != nullptr && ImGui_ImplIWindow_ShouldChainCallback(window)) data->PrevUserCallbackMouseMove(window, x, y);

	ImGuiIO& io = ImGui::GetIO();
	io.AddMousePosEvent((float)x, (float)y);
	data->lastValidMousePos = ImVec2((float)x, (float)y);
}

void ImGui_ImplIWindow_CursorEnterCallback(IWindow::Window& window, bool entered) {

}

void ImGui_ImplIWindow_InstallCallbacks(IWindow::Window& window) {
	ImGui_ImplIWindow_Data* data = ImGui_ImplIWindow_GetBackendData();
	if (data->installedCallbacks) {
		std::cout << "ImGui_Impl_IWindow: Callbacks already installed. Did you call ImGui_ImplIWindow_Init twice?\n";
	}

}
