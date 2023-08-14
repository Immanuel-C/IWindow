/*
    BSD 2-Clause License

    Copyright (c) 2022, Immanuel Charles
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#if defined(__APPLE__)

#include "IWindow.h"

#include <iostream>

#import <Cocoa/Cocoa.h>

//=====================Cocoa OBJECTS=====================
@interface CocoaAppDelegate : NSObject <NSApplicationDelegate> {
    
}

- (void) menuItemClicked:(id) sender;

@end

@implementation CocoaAppDelegate

- (void) menuItemClicked:(id) sender {
    
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {
    return NSTerminateCancel;
}

- (void)applicationDidChangeScreenParameters:(NSNotification *) notification {
    
}

- (void)applicationWillFinishLaunching:(NSNotification *)notification {
    
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    [NSApp stop:nil];
}

- (void)applicationDidHide:(NSNotification *)notification {
    
}

@end

@interface CocoaHelper : NSObject
@end

@implementation CocoaHelper

- (void)selectedKeyboardInputSourceChanged:(NSObject* )object {
    
}

- (void)doNothing:(id)object {
}

@end

static const NSRange nsEmptyRange = { NSNotFound, 0 };

@interface CocoaWindowDelegate : NSObject {
    IWindow::Window* window;
}

- (instancetype)initWithIWindow:(IWindow::Window*)initWindow;

@end

@implementation CocoaWindowDelegate

- (instancetype)initWithIWindow:(IWindow::Window*)initWindow {
    self = [super init];
    if (self != nil) {
        window = initWindow;
    }

    return self;
}

- (BOOL)windowShouldClose:(id)sender {
    window->_setRunning(false);

    return NO;
}

- (void)windowDidResize:(NSNotification *)notification {
    //const NSRect contentRect = [window->view frame];
    //const NSRect fbRect = [window->view convertRectToBacking:contentRect];

    //TODO
}

- (void)windowDidMove:(NSNotification*)notification {
    
}

- (void)windowDidMiniaturize:(NSNotification*)notification {
    
}

- (void)windowDidDeminiaturize:(NSNotification*)notification {
    
}

- (void)windowDidBecomeKey:(NSNotification*)notification {
    
}

- (void)windowDidResignKey:(NSNotification*)notification {
    
}

- (void)windowDidChangeOcclusionState:(NSNotification*)notification {
    
}

@end

//View

@interface CocoaContentView : NSView <NSTextInputClient> {
    IWindow::Window* window;
    NSTrackingArea* trackingArea;
    NSMutableAttributedString* markedText;
}

- (instancetype)initWithIWindow:(IWindow::Window*)initWindow;

@end

@implementation CocoaContentView

- (instancetype)initWithIWindow:(IWindow::Window*)initWindow {
    self = [super init];
    if (self != nil) {
        window = initWindow;
        trackingArea = nil;
        markedText = [[NSMutableAttributedString alloc] init];

        [self updateTrackingAreas];
        [self registerForDraggedTypes:@[NSPasteboardTypeURL]];
    }

    return self;
}

- (void)dealloc {
    [trackingArea release];
    [markedText release];
    [super dealloc];
    [super release];
}

- (BOOL)isOpaque {
    return YES;
}

- (BOOL)canBecomeKeyView {
    return YES;
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (BOOL)wantsUpdateLayer {
    return YES;
}

- (void)updateLayer {
    
}

- (void)cursorUpdate:(NSEvent*)event {
    
}

- (BOOL)acceptsFirstMouse:(NSEvent*)event {
    return YES;
}

- (void)mouseDown:(NSEvent*)event {
    //TODO
}

- (void)mouseDragged:(NSEvent*)event {
    [self mouseMoved:event];
}

- (void)mouseUp:(NSEvent*)event {
    //TODO
}

- (void)mouseMoved:(NSEvent*)event {
    //TODO
}

- (void)rightMouseDown:(NSEvent*)event {
    //TODO
}

- (void)rightMouseDragged:(NSEvent*)event {
    [self mouseMoved:event];
}

- (void)rightMouseUp:(NSEvent*)event {
    //TODO
}

- (void)otherMouseDown:(NSEvent*)event {
    
}

- (void)otherMouseDragged:(NSEvent*)event {
    [self mouseMoved:event];
}

- (void)otherMouseUp:(NSEvent*)event {
    
}

- (void)mouseExited:(NSEvent*)event {
    //TODO
}

- (void)mouseEntered:(NSEvent*)event {
    //TODO
}

- (void)viewDidChangeBackingProperties {
    //const NSRect contentRect = [window->view frame];
    //const NSRect fbRect = [window->view convertRectToBacking:contentRect];
    //TODO
}

- (void)drawRect:(NSRect)rect {
    
}

- (void)updateTrackingAreas {
    if (trackingArea != nil) {
        [self removeTrackingArea:trackingArea];
        [trackingArea release];
    }

    const NSTrackingAreaOptions options = NSTrackingMouseEnteredAndExited |
                                          NSTrackingActiveInKeyWindow |
                                          NSTrackingEnabledDuringMouseDrag |
                                          NSTrackingCursorUpdate |
                                          NSTrackingInVisibleRect |
                                          NSTrackingAssumeInside;

    trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
                                                options:options
                                                  owner:self
                                               userInfo:nil];

    [self addTrackingArea:trackingArea];
    [super updateTrackingAreas];
}

- (void)keyDown:(NSEvent *)event {
    //TODO
}

- (void)flagsChanged:(NSEvent *)event {
    //TODO
}

- (void)keyUp:(NSEvent*)event {
    //TODO
}

- (void)scrollWheel:(NSEvent*)event {
    //TODO
}

- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender {
    return NSDragOperationGeneric;
}

- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender {
    return YES;
}

- (BOOL)hasMarkedText {
    return [markedText length] > 0;
}

- (NSRange)markedRange {
    if ([markedText length] > 0)
        return NSMakeRange(0, [markedText length] - 1);
    else
        return nsEmptyRange;
}

- (NSRange)selectedRange {
    return nsEmptyRange;
}

- (void)setMarkedText:(id)string
        selectedRange:(NSRange)selectedRange
     replacementRange:(NSRange)replacementRange {
    [markedText release];
    if ([string isKindOfClass:[NSAttributedString class]])
        markedText = [[NSMutableAttributedString alloc] initWithAttributedString:string];
    else
        markedText = [[NSMutableAttributedString alloc] initWithString:string];
}

- (void)unmarkText {
    [[markedText mutableString] setString:@""];
}

- (NSArray*)validAttributesForMarkedText {
    return [NSArray array];
}

- (NSAttributedString*)attributedSubstringForProposedRange:(NSRange)range
                                               actualRange:(NSRangePointer)actualRange {
    return nil;
}

- (NSUInteger)characterIndexForPoint:(NSPoint)point {
    return 0;
}

- (NSRect)firstRectForCharacterRange:(NSRange)range
                         actualRange:(NSRangePointer)actualRange {
    const NSRect frame = [window->_getNSView() frame];
    
    return NSMakeRect(frame.origin.x, frame.origin.y, 0.0, 0.0);
}

- (void)insertText:(id)string replacementRange:(NSRange)replacementRange {
    //TODO
}

- (void)doCommandBySelector:(SEL)selector {

}

@end

//Window Object

@interface CocoaWindow : NSWindow {}
@end

@implementation CocoaWindow

- (BOOL)canBecomeKeyWindow {
    return YES;
}

- (BOOL)canBecomeMainWindow {
    return YES;
}

@end

namespace IWindow {

    X11Display Window::GetX11Display() { return nullptr; }

    Window::Window(int64_t width, int64_t height, const std::string& title, int64_t x, int64_t y) { Create(width, height, title, x, y); }
    Window::~Window() { 
        //TODO
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

        //m_icon = ;
        //m_cursor = ;

        //=====================Helper=====================
        CocoaHelper* helper = [[CocoaHelper alloc] init];
        
        [NSThread detachNewThreadSelector:@selector(doNothing:)
                                toTarget:helper
                            withObject:nil];

        [NSApplication sharedApplication];

        //=====================App delegate=====================
        CocoaAppDelegate* appDelegate = [[CocoaAppDelegate alloc] init];
        [NSApp setDelegate:appDelegate];

        NSEvent* (^block)(NSEvent*) = ^ NSEvent* (NSEvent* event) {
            if ([event modifierFlags] & NSEventModifierFlagCommand)
                [[NSApp keyWindow] sendEvent:event];

            return event;
        };

        [[NSNotificationCenter defaultCenter]
            addObserver:helper
            selector:@selector(selectedKeyboardInputSourceChanged:)
                name:NSTextInputContextKeyboardSelectionDidChangeNotification
                object:nil];
        
        CGEventSourceSetLocalEventsSuppressionInterval(CGEventSourceCreate(kCGEventSourceStateHIDSystemState), 0.0);

        //if (![[NSRunningApplication currentApplication] isFinishedLaunching])
        //    [NSApp run];

        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

        //=====================Window delegate=====================
        m_windowDelegate = [[CocoaWindowDelegate alloc] initWithIWindow:this];
        if (m_windowDelegate == nil) {
            //fprintf(stderr, "Failed to create NS window delegate\n");
            return false;
        }

        //=====================Window=====================
        NSRect contentRect = NSMakeRect(0, 0, m_width, m_height);

        NSUInteger styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;
        m_window = [[CocoaWindow alloc]
            initWithContentRect:contentRect
                    styleMask:styleMask
                        backing:NSBackingStoreBuffered
                        defer:NO];
        if (m_window == nil) {
            //fprintf(stderr, "Failed to create NS window\n");
            return false;
        }
        
        [(NSWindow*)m_window center];

        const NSWindowCollectionBehavior behavior =
            NSWindowCollectionBehaviorFullScreenPrimary |
            NSWindowCollectionBehaviorManaged;
        [m_window setCollectionBehavior:behavior];

        //=====================View=====================
        m_view = [[CocoaContentView alloc] initWithIWindow:this];
        if (m_view == nil) {
            //fprintf(stderr, "Failed to create NS view\n");
            return false;
        }

        [m_window setContentView:m_view];
        [m_window makeFirstResponder:m_view];
        [m_window setDelegate:m_windowDelegate];
        [m_window setAcceptsMouseMovedEvents:YES];
        [m_window setRestorable:NO];

        [NSApp activateIgnoringOtherApps:YES];
        [m_window makeKeyAndOrderFront:m_view];

        [m_window orderFrontRegardless];
        [NSApp run];

        return true;
    }

    void Window::Update() {
        @autoreleasepool {
    
        while (true) {
            NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                                untilDate:[NSDate distantPast]
                                                inMode:NSDefaultRunLoopMode
                                                dequeue:YES];
            if (event == nil)
                break;

            [NSApp sendEvent:event];
        }

        } // autoreleasepool
    }

    bool Window::IsRunning() { return m_running; }

    NativeWindowHandle& Window::GetNativeWindowHandle() { return m_window; };

    IVector2 Window::GetWindowSize() {
        //TODO

        return {};
    }

    void Window::SetWindowSize(int64_t width, int64_t height) {
        m_width = width;
        m_height = height;

        //TODO
    }

    void Window::SetWindowPosition(int64_t x, int64_t y) {
        m_x = x;
        m_y = y;

        //TODO
    }

    bool Window::IsKeyDown(Key key) { return m_keys[(int64_t)key]; }

    bool Window::IsKeyUp(Key key) { return !IsKeyDown(key); }

    bool Window::IsMouseButtonDown(MouseButton button) { return m_mouseButtons[(int)button]; }
    bool Window::IsMouseButtonUp(MouseButton button) { return !IsMouseButtonDown(button); }


    Vector2 Window::GetMouseScrollOffset() {
        return Vector2{ m_scrollOffsetX, m_scrollOffsetY };
    }

    IVector2 Window::GetWindowPosition() {
        //TODO

        return {};
    }

    void Window::SetUserPointer(void* ptr) { m_userPtr = ptr; }

    void* Window::GetUserPointer() { return m_userPtr; }

    void Window::SetPosCallback(WindowPosCallback callback) { m_posCallback = callback; }
    void Window::SetSizeCallback(WindowSizeCallback callback) { m_sizeCallback = callback; }
    void Window::SetKeyCallback(KeyCallback callback) { m_keyCallback = callback; }
    void Window::SetMouseMoveCallback(MouseMoveCallback callback) { m_mouseMovecallback = callback; }
    void Window::SetMouseButtonCallback(MouseButtonCallback callback) { m_mouseButtonCallback = callback; }
    void Window::SetMouseScrollCallback(MouseScrollCallback callback) { m_mouseScrollCallback = callback; }

    IVector2 Window::GetMousePosition() { return IVector2{ m_mouseX, m_mouseY }; }

    Monitor Window::GetPrimaryMonitor() {
        //TODO

        return {};
    }

    std::vector<Monitor> Window::GetAllMonitors() {
        //TODO

        return {};
    }

    void Window::Center(Monitor monitor) {
        SetWindowPosition( ( monitor.size.x - m_width ) / 2 , ( monitor.size.y - m_height ) / 2);
    }

    void Window::Fullscreen(bool fullscreen, Monitor monitor) {
        if (m_fullscreen == fullscreen)
            return;

        m_fullscreen = fullscreen;

        // No fullscreen
        if (!m_fullscreen) {
            //TODO
            return;
        }

        // Fullscreen
        m_oldWidth = m_width;
        m_oldHeight = m_height;

        //TODO
    }

    bool Window::IsFullscreen() { return m_fullscreen; }

    void Window::SetIcon(Image image) {
        //TODO
    }

    void Window::SetCursor(Image image, uint32_t hotX, uint32_t hotY) {
        //TODO
    }

    void Window::SetIcon(NativeIconID iconID) {
        //TODO
    }

    void Window::SetCursor(NativeCursorID cursorID) {
        //TODO
    }

    NativeDeviceContext& Window::GetNativeDeviceContext() { return m_deviceContext; }

    //Cocoa specific
    void Window::_setRunning(bool aRunning) {
        m_running = aRunning;
    }

    id Window::_getNSView() {
        return m_view;
    }
}

#endif