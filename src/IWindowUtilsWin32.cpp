#include "IWindowUtils.h"

#include "IWindow.h"

#include <shellscalingapi.h>

namespace IWindow {
    Monitor Monitor::GetPrimaryMonitor()  {
        // The primary montitors top left corner is always 0, 0
        const POINT p = { 0, 0 };
        HMONITOR hmonitor = ::MonitorFromPoint(p, MONITOR_DEFAULTTOPRIMARY);

        MONITORINFOEX monitorInfo{}; // EX has the monitor name
        monitorInfo.cbSize = sizeof(MONITORINFOEX);
        if (!::GetMonitorInfo(hmonitor, &monitorInfo)) {
            MessageBox(nullptr, L"Failed to get primary monitor!", L"Error", MB_ICONEXCLAMATION | MB_OK);
            return Monitor{};
        }

        Monitor monitor{};

        monitor.size.x = (int32_t)monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
        monitor.size.y = (int32_t)monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
        monitor.position.x = (int32_t)monitorInfo.rcMonitor.left;
        monitor.position.y = (int32_t)monitorInfo.rcMonitor.top;
        int32_t size = ::WideCharToMultiByte(CP_UTF8, 0, monitorInfo.szDevice, -1, nullptr, 0, 0, 0);
        monitor.name.resize(size);
        ::WideCharToMultiByte(CP_UTF8, 0, monitorInfo.szDevice, -1, monitor.name.data(), (int32_t)monitor.name.size() * sizeof(char), 0, 0);

        return monitor;
    }

    static BOOL CALLBACK MonitorCallback(HMONITOR hmonitor, HDC, LPRECT rc, LPARAM lparam) {
        std::vector<Monitor>* monitorsVec = (std::vector<Monitor>*)lparam;

        MONITORINFOEX monitorInfo{}; // EX has the monitor name
        monitorInfo.cbSize = sizeof(MONITORINFOEX);
        IWINDOW_CHECK_ERROR(!::GetMonitorInfo(hmonitor, &monitorInfo), ErrorType::Monitor, ErrorSeverity::Error, "GetMonitorInfo() failed. Failed to get monitor information!", true, false);

        Monitor monitor{};

        monitor.size.x = (size_t)monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
        monitor.size.y = (size_t)monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
        monitor.position.x = (size_t)monitorInfo.rcMonitor.left;
        monitor.position.y = (size_t)monitorInfo.rcMonitor.top;
        int32_t size = ::WideCharToMultiByte(CP_UTF8, 0, monitorInfo.szDevice, -1, nullptr, 0, 0, 0);
        monitor.name.resize(size);
        ::WideCharToMultiByte(CP_UTF8, 0, monitorInfo.szDevice, -1, monitor.name.data(), (int32_t)monitor.name.size() * sizeof(char), 0, 0);

        IWINDOW_CHECK_ERROR(!::GetDpiForMonitor(hmonitor, MDT_EFFECTIVE_DPI, (UINT*)&monitor.dpi.x, (UINT*)&monitor.dpi.y), ErrorType::Monitor, ErrorSeverity::Error, "GetMonitorInfo() failed. Failed to get monitor dpi scale! Are you using Windows 8.1 or higher?", true, false);

        monitorsVec->emplace_back(std::move(monitor));

        return true;
    }

    std::vector<Monitor> Monitor::GetAllMonitors() {
        std::vector<Monitor> monitors{};
        ::EnumDisplayMonitors(nullptr, nullptr, MonitorCallback, (LPARAM)&monitors);
        return monitors;
    }
}