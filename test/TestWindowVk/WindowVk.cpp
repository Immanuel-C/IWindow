#include "IWindow.h"
#include "IWindowVK.h"

#include <iostream>

int main() {
    IWindow::Window window{};
    
    if (!window.Create({ 1280, 720 }, L"Hello IWindow")) {
        std::cout << "Failed to create window!\n";
        return EXIT_FAILURE;
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_API_VERSION_1_0;
    // appInfo.apiVersion = VK_API_VERSION_1_1;
    // appInfo.apiVersion = VK_API_VERSION_1_2;
    // appInfo.apiVersion = VK_API_VERSION_1_3;
    appInfo.pApplicationName = "IWindow Test";
    appInfo.pEngineName = "IWindow Test Engine";
    appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);


    std::vector<const char*> iWindowExtensionNames;
    IWindow::Vk::GetRequiredInstanceExtensions(iWindowExtensionNames);

    VkInstanceCreateInfo instanceInfo{};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = (uint32_t)iWindowExtensionNames.size();
    instanceInfo.ppEnabledExtensionNames = iWindowExtensionNames.data();

    VkInstance instance = nullptr;
    if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS) {
        std::cerr << "Failed to create instance\n";
        return EXIT_FAILURE;
    }

    VkSurfaceKHR surface = nullptr;
    if (IWindow::Vk::CreateSurface(window, instance, surface) != VK_SUCCESS) {
        std::cerr << "Failed to create window surface\n";
        return EXIT_FAILURE;
    }


    std::cout << "Success!\n";


    while (window.IsRunning()) {

        window.Update();
    }
}