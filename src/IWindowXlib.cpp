#include "IWindow.h"
#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

const uint32_t SCREEN_BIT_DEPTH = 24;
// Additional mouse button names for XButtonEvent
static constexpr uint32_t Button6 = 6;
static constexpr uint32_t Button7 = 7;

namespace IWindow {
    X11Display Window::GetX11Display() { return m_display; }

    Window::Window(int64_t width, int64_t height, const std::string& title, int64_t x, int64_t y) { Create(width, height, title, x, y); }

    Window::~Window() { 
        XFreeGC(m_display, m_deviceContext);
        XDestroyWindow(m_display, m_window);
        XCloseDisplay(m_display);
    }

    bool Window::Create(int64_t width, int64_t height, const std::string& title, int64_t x, int64_t y) {
        m_width = width;
        m_height = height;
        m_oldWidth = m_width;
        m_oldHeight = m_height;
        m_x = x;
        m_y = y;
        m_title = title;
        m_userPtr = nullptr;
        const char* displayEnv = getenv("DISPLAY");
        m_display = XOpenDisplay(nullptr);

        if (!m_display) { return false; }

        int root = DefaultRootWindow(m_display);
        int defaultScreen = DefaultScreen(m_display);

        XVisualInfo visInfo = {};
        if (!XMatchVisualInfo(m_display, defaultScreen, SCREEN_BIT_DEPTH, TrueColor, &visInfo)) { return false; }

        XSetWindowAttributes windowAttr;
        windowAttr.background_pixel = 0;
        windowAttr.colormap = XCreateColormap(m_display, root, visInfo.visual, AllocNone);
        uint64_t attributeMask = CWBackPixel | CWColormap;

        m_window = XCreateWindow(
                        m_display, root,
                        x, y,
                        width, height, 0,
                        visInfo.depth, InputOutput,
                        visInfo.visual, attributeMask, &windowAttr
                    );

        if (!m_window) { return false; }

        m_deviceContext = XCreateGC(m_display, m_window, 0, nullptr);

        //subscribe to these input events:
        XSelectInput(m_display, m_window, KeyPressMask | KeyReleaseMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask);

        XStoreName(m_display, m_window, title.c_str());  

        XMapWindow(m_display, m_window);
        XFlush(m_display); 


        //register all internal atoms
        Atom wmDeleteWindow = XInternAtom(m_display, "WM_DELETE_WINDOW", False);
        if(!XSetWMProtocols(m_display, m_window, &wmDeleteWindow, 1)) { return false; } 

        return true;
    }

    void Window::Update() {
        if (!XPending(m_display)) // If no events are pending return because XNextEvent will pause the thread until an event is registered.
            return;
        // Move to next window event (and also check if window is still running)
        XEvent ev;
        XNextEvent(m_display, &ev); // Fetch the next event

        if (ev.xany.window != m_window) // If the event is from another window return.
            return;

        switch (ev.type)
        {
        case KeyPress: {
            if (ev.xkey.keycode < (int)Key::Max) { // Check if higher than mask to avoid mem overflow
                m_keys[ev.xkey.keycode] = true;
                m_keyCallback(*this, (Key)ev.xkey.keycode, InputState::Down);
            }
            break;
        }
        case KeyRelease: {
            if (ev.xkey.keycode < (int)Key::Max) {
                m_keys[ev.xkey.keycode] = false;
                m_keyCallback(*this, (Key)ev.xkey.keycode, InputState::Up);
            }
            break;
        }
        case MotionNotify: {
            if (ev.type == MotionNotify) {
                m_x = ev.xmotion.x;
                m_y = ev.xmotion.y;
                m_mouseMovecallback(*this, m_x, m_y);
            }
            break;
        }
        case ButtonPress: {
            if (ev.xbutton.button < (int)MouseButton::Max) {
                // Button4 = forward on Y axis, Button5 = backwards (towards user) on Y axis
                if (ev.xbutton.button == Button4 || ev.xbutton.button == Button5) {
                    m_scrollOffsetY = ev.xbutton.button == Button4 ? 1.0f : -1.0f;
                    m_scrollOffsetX = 0.0f;
                    m_mouseScrollCallback(*this, m_scrollOffsetX, m_scrollOffsetY);
                    break;
                } 
                // Button6 = forward on X axis, Button7 = backwards (towards user) on X axis
                else if (ev.xbutton.button == Button6 || ev.xbutton.button == Button7) {
                    m_scrollOffsetX = ev.xbutton.button == Button6 ? 1.0f : -1.0f;
                    m_scrollOffsetY = 0.0f;
                    m_mouseScrollCallback(*this, m_scrollOffsetX, m_scrollOffsetY);
                    break;
                }

                m_mouseButtons[(int64_t)ev.xbutton.button] = true;
                m_mouseButtonCallback(*this, (MouseButton)ev.xbutton.button, InputState::Down);
            }
            break;
        }
        case ButtonRelease: {
            if (ev.xbutton.button < (int)MouseButton::Max) {
                m_mouseButtons[(int64_t)ev.xbutton.button] = false;
                m_mouseButtonCallback(*this, (MouseButton)ev.xbutton.button, InputState::Up);
            }
            break;
        }
        case DestroyNotify: {
            XDestroyWindowEvent* e = (XDestroyWindowEvent*) &ev;
            m_running = false;
            break;
        }
        case ClientMessage: {
            XClientMessageEvent* e = (XClientMessageEvent*)&ev;
            if((Atom)e->data.l[0] == XInternAtom(m_display, "WM_DELETE_WINDOW", False)) 
                m_running = false;
            break;
        }
        default:
            break;
        }

    }

    bool Window::IsRunning() { return m_running; }

    NativeWindowHandle& Window::GetNativeWindowHandle() { return m_window; }

    IVector2 Window::GetWindowSize() {
        XWindowAttributes windowAttribs = {};
        XGetWindowAttributes(m_display, m_window, &windowAttribs);

        m_width = windowAttribs.width;
        m_height = windowAttribs.height;

        return IVector2{m_width, m_height};
    }

    IVector2 Window::GetWindowPosition() {
        XWindowAttributes windowAttribs = {};
        XGetWindowAttributes(m_display, m_window, &windowAttribs);

        m_x = windowAttribs.x;
        m_y = windowAttribs.y;

        return IVector2{m_x, m_y};
    }

    void Window::SetWindowSize(int64_t width, int64_t height) {
        m_width = width;
        m_height = height;

        XResizeWindow(m_display, m_window, m_width, m_height);
    }

    void Window::SetWindowPosition(int64_t x, int64_t y) {
        m_x = x;
        m_y = y;

        XMoveWindow(m_display, m_window, m_x, m_y);
    }

    Monitor Window::GetPrimaryMonitor() {
        int snum = DefaultScreen(m_display);
        Monitor monitor{};

        monitor.size.x = DisplayWidth(m_display, snum); 
        monitor.size.y = DisplayHeight(m_display, snum);

        using namespace std::string_literals;

        monitor.name = L"Primary Monitor";

        return monitor;
    }

    std::vector<Monitor> Window::GetAllMonitors() {
        std::vector<Monitor> monitors;

        int monitorCount = XScreenCount(m_display);

        for (int i = 0; i < monitorCount; i++) {
            Screen* screen = ScreenOfDisplay(m_display, i);
            Monitor monitor{};

            monitor.size.x = screen->width; 
            monitor.size.y = screen->height;

            using namespace std::string_literals;

            monitor.name = L"Monitor " + std::to_wstring(i);

            monitors.emplace_back(std::move(monitor));
        }

        return monitors;
    }

    void Window::Center(Monitor monitor) {
        SetWindowPosition( ( monitor.size.x - m_width ) / 2 , ( monitor.size.y - m_height ) / 2 );
    }

    void Window::Fullscreen(bool fullscreen, Monitor monitor) {
        if (m_fullscreen == fullscreen) return;

        m_fullscreen = fullscreen;

        if (fullscreen) {
            m_oldWidth = m_width;
            m_oldHeight = m_height;
            //resize and remove window border
            Atom wmState = XInternAtom(m_display, "_NET_WM_STATE", true);
            Atom wmFullscreen = XInternAtom(m_display, "_NET_WM_STATE_FULLSCREEN", true);
            XChangeProperty(m_display, m_window, wmState, XA_ATOM, 32, PropModeReplace, (unsigned char *)&wmFullscreen, 1);
            SetWindowSize(monitor.size.x, monitor.size.y);
            Center(monitor);
            return;
        }

        SetWindowSize(m_oldWidth, m_oldHeight);
        Center(monitor);
    }

    bool Window::IsFullscreen() { return m_fullscreen; }

    void Window::SetUserPointer(void* ptr) { m_userPtr = ptr; }
    void* Window::GetUserPointer() { return m_userPtr; }

    NativeDeviceContext& Window::GetNativeDeviceContext() { return m_deviceContext; }


    bool Window::IsKeyDown(Key key) { return m_keys[(int64_t)key]; }
    bool Window::IsKeyUp(Key key) { return !IsKeyDown(key); }
    bool Window::IsMouseButtonDown(MouseButton button) { return m_mouseButtons[(int64_t)button]; }
    bool Window::IsMouseButtonUp(MouseButton button) { return !IsMouseButtonDown(button); }
    Vector2 Window::GetMouseScrollOffset() { return Vector2{ m_scrollOffsetX, m_scrollOffsetY }; }



    void Window::SetPosCallback(WindowPosCallback callback) { m_posCallback = callback; }
    void Window::SetSizeCallback(WindowSizeCallback callback) { m_sizeCallback = callback; }
    void Window::SetKeyCallback(KeyCallback callback) { m_keyCallback = callback; }
    void Window::SetMouseMoveCallback(MouseMoveCallback callback) { m_mouseMovecallback = callback; }
    void Window::SetMouseButtonCallback(MouseButtonCallback callback) { m_mouseButtonCallback = callback; }
    void Window::SetMouseScrollCallback(MouseScrollCallback callback) { m_mouseScrollCallback = callback; }
};
