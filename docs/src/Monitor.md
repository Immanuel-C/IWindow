# Monitors

An `IWindow::Monitor` is a struct that represents a real monitor.

- `IWindow::IVector2 size` is the size in pixels of the monitor.  
- `IWindow::IVector2 position` is the position in pixels of the monitor. The top left of the monitor is the position (e.g. on the primary monitor, the position of the top left corner is `0, 0`).
- `std::wstring name` is a name the operating system gave the monitor.  
