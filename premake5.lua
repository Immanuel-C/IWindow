workspace "IWindow"
    configurations {"Debug", "Release"}
    architecture "x86_64"

    print ("Make sure to set the vulkan sdk path")

    vulkanSdk = os.getenv("VULKAN_SDK");

    if vulkanSdk == nil then
        error "The vulkan sdk path is not set!"
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

    if package.config:sub(1,1) == "/" then   --if linux (using "/" seperator")  NOTE: There are other platforms that use this directory seperator too... but the only one IWindow supports is linux at the current moment
    startproject("LinuxTest")

    project "LinuxTest"
        location "test/LinuxTest"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"

        files {"%{prj.location}/WindowX11.cpp", "src/IWindowXlib.cpp", "src/IWindowXlibGL.cpp", "src/IWindowXlibGamepad.cpp"}

        includedirs {"src"}


        -- libdirs { vulkanSdk ..  "/lib" }
        links {"X11", "GLX", "GL"}

        defaultBuildLocation()

        defaultBuildCfg()
    else                            -- else (using windows)

    startproject("TestWindow")
    project "TestWindow"
        location "test/TestWindow"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"

        files {"%{prj.location}/WindowWin32.cpp", "%{prj.location}/stb.cpp", "src/IWindowWin32.cpp", "src/IWindowWin32Gamepad.cpp"}

        includedirs { "src" }

        links {"User32", "XInput"}

        defaultBuildLocation()

        defaultBuildCfg()
        
    project "TestWindowGL"
        location "test/TestWindowGL"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"

        files {"%{prj.location}/WindowWin32GL.cpp", "%{prj.location}/glad.cpp"}

        includedirs { "src", "%{prj.location}/deps/glad/include" }

        links {"User32", "IWindowWin32GL"}

        defaultBuildLocation()

        defaultBuildCfg()

    project "TestWindowVk"
        location "test/TestWindowVk"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"

        files {"%{prj.location}/WindowWin32Vk.cpp"}

        includedirs { vulkanSdk .. "/Include", "src" }

        links {"User32", "IWindowWin32Vk"}

        defaultBuildLocation()

        defaultBuildCfg()

    project "IWindowWin32GL"
        location "src"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"

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

        files {"%{prj.location}/IWindowWin32.cpp", "src/IWindowWin32Vk.cpp", "%{prj.location}/IWindowWin32GL.cpp", "src/IWindowWin32Gamepad.cpp"}

        includedirs { vulkanSdk .. "/Include", "src" }

        libdirs { vulkanSdk .. "/Lib" }
        links {"User32", "vulkan-1", "OpenGL32", "XInput"}

        defaultBuildLocation()

        defaultBuildCfg()

    end