#if !defined(_WIN32)
#include "IWindow.h"
#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xcursor/Xcursor.h>

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
        m_display = XOpenDisplay(nullptr);

        if (!m_display) {
            std::cerr << "Failed to open display connection!\n";
            return false; 
        }

        int root = DefaultRootWindow(m_display);
        int defaultScreen = DefaultScreen(m_display);

        static std::array<int, 23> visualAttribs = {
            GLX_X_RENDERABLE, true,
            GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
            GLX_RENDER_TYPE, GLX_RGBA_BIT,
            GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
            GLX_RED_SIZE, 8,
            GLX_GREEN_SIZE, 8,
            GLX_BLUE_SIZE, 8,
            GLX_ALPHA_SIZE, 8,
            GLX_DEPTH_SIZE, 24,
            GLX_STENCIL_SIZE, 8,
            GLX_DOUBLEBUFFER, true,
            None
        };

        int fbcount = 0;

        GLXFBConfig* fbc = glXChooseFBConfig(
            m_display, 
            DefaultScreen(m_display), 
            visualAttribs.data(), 
            &fbcount
        );

        if (!fbc) {
            std::cerr << "Failed to retrieve a framebuffer config\n";
            return false;
        }

        int bestFbcIndex = -1, worstFbcIndex = -1, BestNumSamples = -1, worstNumSamples = -1;

        // Choose framebuffer config
        for (int32_t i = 0; i < fbcount; i++) {
            XVisualInfo* visualInfo = glXGetVisualFromFBConfig(m_display, fbc[i]);
            if (visualInfo) {
                int sampleBuffers = 0, samples = 0;
                glXGetFBConfigAttrib(m_display, fbc[i], GLX_SAMPLE_BUFFERS, &sampleBuffers);
                glXGetFBConfigAttrib(m_display, fbc[i], GLX_SAMPLES, &samples);
                
                if ( bestFbcIndex < 0 || sampleBuffers && samples > BestNumSamples )
                    bestFbcIndex = i, BestNumSamples = samples;
                if ( worstFbcIndex < 0 || !sampleBuffers || samples < worstNumSamples )
                    worstFbcIndex = i, worstNumSamples = samples;
            }
            XFree(visualInfo);
        }


        GLXFBConfig bestFbc = fbc[bestFbcIndex];
        XVisualInfo *vi = glXGetVisualFromFBConfig(m_display, bestFbc);

        XSetWindowAttributes windowAttr;
        windowAttr.background_pixel = 0;
        windowAttr.colormap = XCreateColormap(m_display, root, vi->visual, AllocNone);
        uint64_t attributeMask = CWBackPixel | CWColormap ;

        m_window = XCreateWindow(
                        m_display, root,
                        x, y,
                        width, height, 0,
                        vi->depth, InputOutput,
                        vi->visual, attributeMask, &windowAttr
                    );

        if (!m_window) { 
            std::cerr << "Failed to create window!\n";
            return false; 
        }

        m_deviceContext = XCreateGC(m_display, m_window, 0, nullptr);
            //subscribe to these input events:
        XSelectInput(m_display, m_window, 
            KeyPressMask        | 
            KeyReleaseMask      | 
            PointerMotionMask   | 
            ButtonPressMask     | 
            ButtonReleaseMask   | 
            // For resize and move events 
            StructureNotifyMask
        );

        XStoreName(m_display, m_window, title.c_str());  

        XMapWindow(m_display, m_window);
        XFlush(m_display); 


        //register all internal atoms
        Atom wmDeleteWindow = XInternAtom(m_display, "WM_DELETE_WINDOW", False);
        if(!XSetWMProtocols(m_display, m_window, &wmDeleteWindow, 1)) { return false; } 

        std::cout << "Linux joystick driver version: " << JSIOCGVERSION << '\n';

        return true;
    }

    void Window::Update() {
        while (XPending(m_display)) { // If no events are pending return because XNextEvent will pause the thread until an event is registered.
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
                    m_mouseX = ev.xmotion.x;
                    m_mouseY = ev.xmotion.y;
                    m_mouseMovecallback(*this, m_mouseX, m_mouseY);
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
            // Resize and move events
            case ConfigureNotify: {
                if (ev.xconfigure.width != m_width || ev.xconfigure.height != m_height) {
                    m_width = ev.xconfigure.width;
                    m_height = ev.xconfigure.height;
                    m_sizeCallback(*this, m_width, m_height);
                } else if (ev.xconfigure.x != m_x || ev.xconfigure.y != m_y) {
                    m_x = ev.xconfigure.x;
                    m_y = ev.xconfigure.y;
                    m_posCallback(*this, m_y, m_x);
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
                if((Atom)e->data.l[0] == XInternAtom(m_display, "WM_DELETE_WINDOW", false)) 
                    m_running = false;
                break;
            }
            default:
                break;
            }

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

    IVector2 Window::GetMousePosition() {
        return IVector2{ m_mouseX, m_mouseY };
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

    
    void Window::SetIcon(Image image) {
        int longCount = 0;

        longCount += 2 + image.width * image.height;

        unsigned long* icon = new unsigned long[longCount * sizeof(unsigned long)];
        unsigned long* target = icon;

        *target++ = image.width;
        *target++ = image.height;

        for (int i = 0;  i < image.width * image.height;  i++) {
            *target++ = (((unsigned long) image.data[i * 4 + 0]) << 16) |
                        (((unsigned long) image.data[i * 4 + 1]) <<  8) |
                        (((unsigned long) image.data[i * 4 + 2]) <<  0) |
                        (((unsigned long) image.data[i * 4 + 3]) << 24);
        }

        XChangeProperty(m_display, m_window,
                        XInternAtom(m_display, "_NET_WM_ICON", false),
                        XA_CARDINAL, 32,
                        PropModeReplace,
                        (unsigned char*) icon,
                        longCount);

        XFlush(m_display);
    }

    void Window::SetCursor(Image image, uint32_t hotX, uint32_t hotY) {
        XcursorImage* native = XcursorImageCreate(image.width, image.height);
        if (!native)
            return;

        native->xhot = hotX;
        native->yhot = hotY;

        unsigned char* source = (unsigned char*) image.data;
        XcursorPixel* target = native->pixels;

        for (int i = 0;  i < image.width * image.height;  i++, target++, source += 4)
        {
            unsigned int alpha = source[3];

            *target = (alpha << 24) |
                    ((unsigned char) ((source[0] * alpha) / 255) << 16) |
                    ((unsigned char) ((source[1] * alpha) / 255) <<  8) |
                    ((unsigned char) ((source[2] * alpha) / 255) <<  0);
        }

        m_cursor = XcursorImageLoadCursor(m_display, native);
        XcursorImageDestroy(native);

        XDefineCursor(m_display, m_window, m_cursor);
    }

    // Win32 (Windows) Only
    void Window::SetIcon(NativeIconID iconID) {}
    // Win32 (Windows) Only
    void Window::SetCursor(NativeCursorID cursorID) {}

}; 
#endif // _WIN32