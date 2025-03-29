#!lua

includeDirList = {
    "../external",
}

libDirectories = { 
    "../lib"
}


if os.get() == "macosx" then
    linkLibs = {
        "graphics-framework",
        "lua",
		"lodepng"
    }
end

if os.get() == "linux" then
    linkLibs = {
        "graphics-framework",
        "lua",
        "lodepng",
        "stdc++",
        "dl",
        "pthread"
    }
end

if os.get() == "macosx" then
    linkOptionList = { "-framework IOKit", "-framework Cocoa", "-framework CoreVideo", "-framework OpenGL" }
end

buildOptions = {"-std=c++14 -O2"}

solution "CS488-Projects"
    configurations { "Debug", "Release" }

    project "RayTracer"
        kind "ConsoleApp"
        language "C++"
        location "build"
        objdir "build"
        targetdir "."
        buildoptions (buildOptions)
        libdirs (libDirectories)
        links (linkLibs)
        linkoptions (linkOptionList)
        includedirs (includeDirList)
        files { "*.cpp" }

    configuration "Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    configuration "Release"
        defines { "NDEBUG" }
        flags { "Optimize" }
