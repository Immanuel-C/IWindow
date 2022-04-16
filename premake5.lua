workspace "IWindow"
    configurations {"Debug", "Release"}
    architecture "x86_64"

    useVulkan = true
    vulkanSdk = ""

    if useVulkan then
        vulkanSdk = os.getenv("VULKAN_SDK");
        if vulkanSdk == nil then
            error "The vulkan sdk path is not set!"
        end
    end

    function defaultBuildCfg()
        filter "configurations:Debug"
            defines { "DEBUG" }
            symbols "On"
            runtime "Debug"
            optimize "Debug"

        filter "configurations:Release"
            defines { "NDEBUG" }
            symbols "Off"
            runtime "Release"
            optimize "Speed"
    end

    function defaultBuildLocation()
        targetdir ("bin/%{prj.name}/%{cfg.buildcfg}")
        objdir ("bin-int/%{prj.name}/%{cfg.buildcfg}")
    end

    project "TestWindow"
        location "test/TestWindow"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"

        files {"%{prj.location}/WindowWin32.cpp", "src/IWindowWin32.cpp"}

        includedirs { "src" }

        links {"User32"}

        defaultBuildLocation()

        defaultBuildCfg()
        
    project "TestWindowGL"
        location "test/TestWindowGL"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"

        files {"%{prj.location}/WindowWin32GL.cpp", "src/IWindowWin32.cpp", "src/IWindowWin32GL.cpp"}

        includedirs { "src" }

        links {"User32", "OpenGL32.lib"}

        defaultBuildLocation()

        defaultBuildCfg()

    project "TestWindowVk"
        location "test/TestWindowVk"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"

        files {"%{prj.location}/WindowWin32Vk.cpp", "src/IWindowWin32.cpp", "src/IWindowWin32Vk.cpp"}

        includedirs { vulkanSdk .. "/Include", "src" }

        libdirs { vulkanSdk .. "/Lib" }
        links {"User32", "vulkan-1"}

        defaultBuildLocation()

        defaultBuildCfg()
    
        

    