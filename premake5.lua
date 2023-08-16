workspace "IWindow"
    configurations {"Debug", "Release"}
    architecture "x86_64"

    print ("Make sure to set the vulkan sdk path!")

    vulkanSdk = os.getenv("VULKAN_SDK");

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

    startproject "TestWindow"

    project "TestWindow"
        location "test/TestWindow"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"

        files {"%{prj.location}/Window.cpp", "%{prj.location}/stb.cpp", "src/IWindowWin32.cpp", "src/IWindowWin32Gamepad.cpp"}

        includedirs { "src" }

        links { "User32", "XInput" } 

        defaultBuildLocation()

        defaultBuildCfg()
        
    project "TestWindowGL"
        location "test/TestWindowGL"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"

        files {
            "%{prj.location}/WindowGL.cpp", 
            "%{prj.location}/glad.cpp", 
            "%{prj.location}/../../deps/imgui/backends/imgui_impl_opengl3.cpp", 
            "%{prj.location}/../../deps/imgui/imgui_draw.cpp", 
            "%{prj.location}/../../deps/imgui/imgui_tables.cpp",
            "%{prj.location}/../../deps/imgui/imgui_widgets.cpp",
            "%{prj.location}/../../deps/imgui/imgui_demo.cpp",
            "%{prj.location}/../../src/IWindowImGUIBackend.cpp",
        }

        includedirs { "src", "%{prj.location}/deps/glad/include", "%{prj.location}/../../deps/imgui", "%{prj.location}/../../deps/imgui/backends"}


        links {"IWindowWin32GL", "OpenGL32", "ImGui"}

        defaultBuildLocation()

        defaultBuildCfg()

    project "TestWindowVk"
        location "test/TestWindowVk"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"

        files {"%{prj.location}/WindowVk.cpp"}
        
        
        if package.config:sub(1,1) == "/" then -- Linux
            platformLinks = { "IWindowXlibVk", "X11", "Xcursor", "vulkan", "GLX" }
            includedirs { "src" }
        else
            libdirs { vulkanSdk .. "/Lib" }
            includedirs { vulkanSdk .. "/Include", "src" }
            platformLinks = { "IWindowWin32Vk", "User32", "vulkan-1" }
        end

        links { platformLinks }

        defaultBuildLocation()

        defaultBuildCfg()

    project "IWindowWin32GL"
        location "src"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"

        includedirs { "src" }

        files {"%{prj.location}/IWindowWin32.cpp", "%{prj.location}/IWindowWin32GL.cpp", "src/IWindowWin32Gamepad.cpp"}

        links {"User32", "OpenGL32", "XInput"}

        defaultBuildLocation()

        defaultBuildCfg()


    project "IWindowWin32Vk"
        location "src"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"

        files {"%{prj.location}/IWindowWin32.cpp", "%{prj.location}/IWindowWin32Vk.cpp", "src/IWindowWin32Gamepad.cpp"}

        includedirs { vulkanSdk .. "/Include", "src" }

        libdirs { vulkanSdk .. "/Lib" }
        links {"User32", "vulkan-1", "XInput"}

        defaultBuildLocation()

        defaultBuildCfg()

    project "IWindowWin32All"
        location "src"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"

        files {"%{prj.location}/**.cpp", "%{prj.location}/**.h", "%{prj.location}/**.hpp"}

        includedirs { vulkanSdk .. "/Include", "src", "%{prj.location}/../deps/imgui/", "%{prj.location}/../deps/imgui/backends"}

        libdirs { vulkanSdk .. "/Lib" }
        links {"User32", "vulkan-1", "OpenGL32", "XInput", "ImGui"}

        defaultBuildLocation()

        defaultBuildCfg()

    project "ImGui"
        location "deps/imgui"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"

        files {
            "%{prj.location}/imgui.cpp", 
            "%{prj.location}/imgui.h", 
            "%{prj.location}/backends/imgui_impl_win32.cpp", 
            "%{prj.location}/backends/imgui_impl_win32.h", 
        }

        includedirs {
            "%{prj.location}"
        }

        links {"User32"}

        defaultBuildLocation()

        defaultBuildCfg()


