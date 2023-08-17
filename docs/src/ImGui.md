# ImGui With IWindow

### What is ImGui?

ImGui is a GUI library that allows you to draw custom windows inside of the main window and create widgets and useful tools to streamline the game making process.

If you have used ImGui with GLFW it the IWindow implementation should be the same just replace `ImGui_ImplGlfw_XXXX` with `ImGui_ImplIWindow_XXXX`. 

This is what the ImGui Backend header file looks like
```cpp

...

bool ImGui_ImplIWindow_Init(IWindow::Window& window, bool installCallbacks = true);
void ImGui_ImplIWindow_NewFrame();
void ImGui_ImplIWindow_Shutdown();

...

```
Some stuff is left out but its not important.

### Initializing ImGui with IWindow

This function Initializing ImGui with IWindow.

`bool ImGui_ImplIWindow_Init(IWindow::Window& window, bool installCallbacks = true);`
- The `IWindow::Window` arguemnt is the window we want to draw ImGui to.
- The `bool` argument is for advanced users only that want to install there own custom ImGui callbacks. 

You should set all IWindow callbacks before calling this function.

### New Frame

This will update important info for ImGui call this every frame.

`void ImGui_ImplIWindow_NewFrame();`

### Shutdown

Shuts down ImGui's connection with IWindow. Call this after main loop during cleanup.

`void ImGui_ImplIWindow_Shutdown();`
