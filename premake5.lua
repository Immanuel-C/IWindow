workspace "IWindow"
    configurations {"Debug", "Release"}
    architecture "x86_64"

    print ("Make sure to set the vulkan sdk path")

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

    startproject("TestWindow")
    project "TestWindow"
        location "test/TestWindow"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"

        platformLinks = nil
        platformFiles = nil

        if package.config:sub(1,1) == "/" then -- Linux
            platformFiles = {"src/IWindowXlib.cpp", "src/IWindowXlibGamepad.cpp"}
            platformLinks = {"X11", "Xcursor", "GLX"}
        else
            platformFiles = {"src/IWindowWin32.cpp", "src/IWindowWin32Gamepad.cpp"}
            platformLinks = {"User32", "XInput"}
        end

        files {"%{prj.location}/Window.cpp", "%{prj.location}/stb.cpp", platformFiles}

        includedirs { "src" }

        links { platformLinks } 

        defaultBuildLocation()

        defaultBuildCfg()
        
    project "TestWindowGL"
        location "test/TestWindowGL"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"

        files {"%{prj.location}/WindowGL.cpp", "%{prj.location}/glad.cpp"}

        includedirs { "src", "%{prj.location}/deps/glad/include" }

        platformLinks = nil

        if package.config:sub(1,1) == "/" then -- Linux
            platformLinks = { "IWindowXlibGL", "GLX", "GL", "X11", "Xcursor"}
        else
            platformLinks = { "IWindowWin32GL", "OpenGL32" }
        end

        links {platformLinks}

        defaultBuildLocation()

        defaultBuildCfg()

    project "TestWindowVk"
        location "test/TestWindowVk"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"

        files {"%{prj.location}/WindowVk.cpp"}
        includedirs { "src" }

        if not vulkanSdk  == nil then
            includedirs { vulkanSdk .. "/Include"}
        end

        if package.config:sub(1,1) == "/" then -- Linux
            platformLinks = { "IWindowXlibVk", "X11", "Xcursor", "vulkan", "GLX" }
        else
            platformLinks = { "IWindowWin32Vk", "User32", "vulkan-1" }
        end

        links {platformLinks}

        defaultBuildLocation()

        defaultBuildCfg()

    if package.config:sub(1,1) == "/" then   --if linux (using "/" seperator")  NOTE: There are other platforms that use this directory seperator too... but the only one IWindow supports is linux at the current moment
    project "IWindowXlibGL"
        location "src"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"

        files {"%{prj.location}/IWindowXlib.cpp", "%{prj.location}/IWindowXlibGL.cpp", "src/IWindowXlibGamepad.cpp"}

        includedirs { "src" }

        links {"X11", "GLX", "GL", "Xcursor"}

        defaultBuildLocation()

        defaultBuildCfg()
    
    project "IWindowXlibVK"
        location "src"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"

        includedirs { "src" }


        files {"%{prj.location}/IWindowXlib.cpp", "%{prj.location}/IWindowXlibVk.cpp", "src/IWindowXlibGamepad.cpp"}
        
        links {"X11",  "Xcursor", "vulkan"}

        defaultBuildLocation()

        defaultBuildCfg()

    project "IWindowXlibAll"
        location "src"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"

        includedirs { "src" }

        files {"%{prj.location}/IWindowXlib.cpp", "%{prj.location}/IWindowXlibVk.cpp", "%{prj.location}/IWindowXlibGL.cpp", "src/IWindowXlibGamepad.cpp"}

        links {"X11",  "Xcursor", "vulkan", "GL", "GLX"}

        defaultBuildLocation()

        defaultBuildCfg()
    else                            -- else (using windows)

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

        files {"%{prj.location}/IWindowWin32.cpp", "src/IWindowWin32Vk.cpp", "%{prj.location}/IWindowWin32GL.cpp", "src/IWindowWin32Gamepad.cpp"}

        includedirs { vulkanSdk .. "/Include", "src" }

        libdirs { vulkanSdk .. "/Lib" }
        links {"User32", "vulkan-1", "OpenGL32", "XInput"}

        defaultBuildLocation()

        defaultBuildCfg()

    end

    