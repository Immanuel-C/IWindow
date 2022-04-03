#include "IWindow.h"
#include "IWindowVk.h"

#include <iostream>

void WindowPosCallback(IWindow::Window& window, uint32_t x, uint32_t y) {
    std::cout << x << ", " << y << '\n';
}

int main() {
    IWindow::Window window = IWindow::Window(1280, 720, "Hello IWindow");

    window.SetPosCallback(WindowPosCallback);

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_API_VERSION_1_2;
    appInfo.pApplicationName = "IWindow Test";
    appInfo.pEngineName = "IWindow Test Engine";
    appInfo.engineVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
    appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);


    std::vector<const char*> iWindowExtensionNames;
    IWindow::Vk::GetRequiredInstanceExtensions(iWindowExtensionNames);


    VkInstanceCreateInfo instanceInfo{};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = (uint32_t)iWindowExtensionNames.size();
    instanceInfo.ppEnabledExtensionNames = iWindowExtensionNames.data();

    VkInstance instance;
    if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS) {
        std::cerr << "Failed to create instance\n";
        return EXIT_FAILURE;
    }

    VkSurfaceKHR surface;
    if (IWindow::Vk::CreateSurface(instance, surface, window) != VK_SUCCESS) {
        std::cerr << "Failed to create window surface\n";
        return EXIT_FAILURE;
    }


    while (window.IsRunning()) {

        window.Update();
    }
}