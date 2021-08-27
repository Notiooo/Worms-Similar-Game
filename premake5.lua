workspace "Worms-Clone"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

    filter "configurations:Debug"
        defines "_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "_RELEASE"
        runtime "Release"
        optimize "On"
        
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Worms-Clone"
    location "Worms-Clone"
    kind "ConsoleApp"
    language "C++"

    targetdir("bin/" .. outputdir .. "/%{wks.name}")
    objdir("bin-int/" .. outputdir .. "/%{wks.name}")

    pchheader "pch.h"
    pchsource "%{wks.name}/src/pch.cpp"

    files
    {
        "%{wks.name}/src/**.h",
        "%{wks.name}/src/**.cpp"
    }

    includedirs
    {
        "%{wks.name}/vendor/include",
        "%{wks.name}/src"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "SFML_STATIC"
        }

        libdirs
        {
            "%{wks.name}/vendor/lib/box2d/windows",
            "%{wks.name}/vendor/lib/gtest/windows",
            "%{wks.name}/vendor/lib/SFML/windows",
            "%{wks.name}/vendor/lib/poly2tri/windows",
            "%{wks.name}/vendor/lib/clipper/windows"
        }

        links
        {
            "box2d",
            "poly2tri",
            "clipper",

            -- SFML
            "opengl32",
            "freetype",
            "winmm",
            "gdi32",
            "openal32",
            "flac",
            "vorbisenc",
            "vorbisfile",
            "vorbis",
            "ogg"
        }

        postbuildcommands
        {
            ('{COPYFILE} ../%{wks.name}/vendor/lib/SFML/windows/openal32.dll %{cfg.buildtarget.bundlepath}'),
            ('{COPYDIR} ../%{wks.name}/resources %{cfg.buildtarget.bundlepath}/resources')
        }

        filter {"configurations:Debug", "system:windows"}
            links { "sfml-audio-s-d", "sfml-graphics-s-d", "sfml-network-s-d", "sfml-system-s-d", "sfml-window-s-d" }

        filter {"configurations:Release", "system:windows"}
            links { "sfml-audio-s", "sfml-graphics-s", "sfml-network-s", "sfml-system-s", "sfml-window-s" }


    filter "system:linux"
        cppdialect "C++17"

        libdirs
        {
            "%{wks.name}/vendor/lib/box2d/linux",
            "%{wks.name}/vendor/lib/gtest/linux",
            "%{wks.name}/vendor/lib/SFML/linux",
            "%{wks.name}/vendor/lib/poly2tri/linux",
            "%{wks.name}/vendor/lib/clipper/linux"
        }

        links { "box2d", "clipper", "poly2tri", "sfml-audio", "sfml-graphics", "sfml-network", "sfml-system", "sfml-window" }
        linkoptions { '-Wl,-rpath=./libs' }
        
        postbuildcommands
        {
            ('{MKDIR} %{cfg.buildtarget.bundlepath}/libs'),
            ('{COPYFILE} ../%{wks.name}/vendor/lib/SFML/linux/* %{cfg.buildtarget.bundlepath}/libs'),
            ('{COPYDIR} ../%{wks.name}/resources %{cfg.buildtarget.bundlepath}')
        }
        

