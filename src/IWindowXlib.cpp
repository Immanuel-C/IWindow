#include "IWindow.h"
#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

const uint SCREEN_BIT_DEPTH = 24;

namespace IWindow {
    X11Display Window::GetX11Display() { return m_display; }

    Window::Window(int64_t width, int64_t height, const std::string& title, int64_t x, int64_t y) { Create(width, height, title, x, y); }

    Window::~Window() { 
        XDestroyWindow(reinterpret_cast<::Display*>(m_display), m_window);
    }

    bool Window::Create(int64_t width, int64_t height, const std::string& title, int64_t x, int64_t y) {
        ::Display* display = XOpenDisplay(0);

        if (!display)
        {
            return false;
        }

        int root = DefaultRootWindow(display);
        int defaultScreen = DefaultScreen(display);

        XVisualInfo visInfo = {};
        if (!XMatchVisualInfo(display, defaultScreen, SCREEN_BIT_DEPTH, TrueColor, &visInfo))
        {
            return false;
        }

        XSetWindowAttributes windowAttr;
        windowAttr.background_pixel = 0;
        windowAttr.colormap = XCreateColormap(display, root, visInfo.visual, AllocNone);
        uint64_t attributeMask = CWBackPixel | CWColormap;

        m_window = XCreateWindow(display, root,
                              x, y,
                              width, height, 0,
                              visInfo.depth, InputOutput,
                              visInfo.visual, attributeMask, &windowAttr);

        if (!m_window)
        {
            return false;
        }

        //subscribe to these input events:
        XSelectInput(display, m_window, KeyPressMask | KeyRelease);

        XStoreName(display, m_window, title.c_str());  

        XMapWindow(display, m_window);
        XFlush(display); 

        //register all internal atoms
        WmCleanup = XInternAtom(display, "WM_DELETE_WINDOW", False);
        if(!XSetWMProtocols(display, m_window, &WmCleanup, 1))
        {   
            return false;
        } 

        WindowType = XInternAtom (display, "_NET_WM_WINDOW_TYPE", false );
        _value = XInternAtom (display, "_NET_WM_WINDOW_TYPE_DOCK", false );

        m_display = display;

        return true;
    }

    void Window::Update() {
          
        //move to next window event (and also check if window is still running)
        XEvent ev;
        XNextEvent(reinterpret_cast<::Display*>(m_display), &ev); //fetch the next event

            //key input events
          if(ev.type == KeyPress) {
                if (ev.xkey.keycode < static_cast<int>(Key::Max)) //check if higher than mask to avoid mem overflow
                {
                    m_keys[ev.xkey.keycode] = true;
                }
          }
          
          if (ev.type == KeyRelease) {
                if (ev.xkey.keycode < static_cast<int>(Key::Max))
                {
                    m_keys[ev.xkey.keycode] = false;
                }
          }

          //window destruction events
          if (ev.type == DestroyNotify) {   //force destroy if not supported on platform
            XDestroyWindowEvent* e = (XDestroyWindowEvent*) &ev;
            if(e->window == m_window)
            {
                m_running = false;
            }

          }
          if (ev.type == ClientMessage) {  //we have already subscribed to call an X11 ClientMessage on window destruction (without destroying the actual window)
                    XClientMessageEvent* e = (XClientMessageEvent*)&ev;
                    if((Atom)e->data.l[0] == WmCleanup)
                    {
                        m_running = false;
                    }

          }
        
        
    }

    bool Window::IsRunning() { return m_running; }

    NativeWindowHandle& Window::GetNativeWindowHandle() { return m_window; }

    IVector2 Window::GetWindowSize() {
        XWindowAttributes window_attribs = {};
        XGetWindowAttributes(reinterpret_cast<::Display*>(m_display), m_window, &window_attribs);

        return IVector2{window_attribs.width, window_attribs.height};
    }

    void Window::SetWindowSize(int64_t width, int64_t height) {
        m_width = width;
        m_height = height;

        XResizeWindow(reinterpret_cast<::Display*>(m_display), m_window, width, height);
    }

    void Window::SetWindowPosition(int64_t x, int64_t y) {
        m_x = x;
        m_y = y;

        XMoveWindow(reinterpret_cast<::Display*>(m_display), m_window, x, y);
    }

      Monitor Window::GetPrimaryMonitor() {
        ::Display* display = reinterpret_cast<::Display*>(m_display);
        int snum = DefaultScreen(display);
        Monitor monitor{};

        monitor.size.x = DisplayWidth(display, snum); 
        monitor.size.y = DisplayHeight(display, snum);
        monitor.position.x = 0;
        monitor.position.y = 0;
        monitor.name = std::to_wstring(snum);

        return monitor;
    }

    std::vector<Monitor> Window::GetAllMonitors() {
        std::vector<Monitor> monitors;
        monitors.push_back(GetPrimaryMonitor());
        return monitors;
    }


    void Window::Center(Monitor monitor) {
        SetWindowPosition( ( monitor.size.x - m_width ) / 2 , ( monitor.size.y - m_height ) / 2);
    }

    void Window::Fullscreen(bool fullscreen, Monitor monitor) {
    
    //move window to another monitor (x11 screen)
    if (fullscreen == true)
    {
        //resize and remove window border
        XResizeWindow(reinterpret_cast<::Display*>(m_display), m_window, monitor.size.x, monitor.size.y);
        XChangeProperty(reinterpret_cast<::Display*>(m_display), m_window, WindowType, XA_ATOM, 32, PropModeReplace, (unsigned char *) &_value, 1);
    
        //Center window (this will only PROPERLY work if the window is fullscreen...)
        SetWindowPosition(0 ,0);
    }
    else
    {
        XDestroyWindow(reinterpret_cast<::Display*>(m_display), m_window);
        Create(m_width, m_height, m_title, m_x, m_y);
    }

    m_fullscreen = fullscreen;
       
    }
    bool Window::IsFullscreen() { return m_fullscreen; }


    void Window::SetUserPointer(void* ptr)
    {
        m_userPtr = ptr;
    }
    void* Window::GetUserPointer()
    {
        return m_userPtr;
    }

    NativeDeviceContext& Window::GetNativeDeviceContext()
    {
        return m_display;
    }

    bool Window::IsKeyDown(Key key)
    {
        return m_keys[(int)key];
    }

    bool Window::IsKeyUp(Key key)
    {
        return !IsKeyDown(key);
    }


};
