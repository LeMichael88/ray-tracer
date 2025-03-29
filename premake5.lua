#!lua

-- Set common variables.
includeDirList = { "external" }
buildOptions = { "-std=c++14", "-O2" }
PLATFORM = os.target()

-- Create lib directory if it does not exist.
if not os.isdir("lib") then
    os.mkdir("lib")
end

-- Build lua-5.4.7 library and copy it into lib if not present.
if not os.isfile("lib/liblua.a") then
    os.chdir("external/lua-5.4.7")
    if PLATFORM == "macosx" then
        os.execute("make macosx")
    elseif PLATFORM == "linux" then
        os.execute("make linux")
    elseif PLATFORM == "windows" then
        os.execute("make mingw")
    end
    os.chdir("../../")
    os.execute("cp external/lua-5.4.7/src/liblua.a lib/")
end

-- Set up linking parameters for the RayTracer project.
libDirectories = { "lib" }
if PLATFORM == "macosx" then
    linkLibs = { "graphics-framework", "lua", "lodepng" }
    linkOptionList = { "-framework IOKit", "-framework Cocoa", "-framework CoreVideo", "-framework OpenGL" }
elseif PLATFORM == "linux" then
    linkLibs = { "graphics-framework", "lua", "lodepng", "stdc++", "dl", "pthread" }
    linkOptionList = {}
else
    linkLibs = {}
    linkOptionList = {}
end

-- Create one solution that builds all projects.
solution "CombinedProject"
    configurations { "Debug", "Release" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter {}

    -- Build graphics-framework static library.
    project "graphics-framework"
        kind "StaticLib"
        language "C++"
        location "build"
        objdir "build"
        targetdir "lib"
        buildoptions (buildOptions)
        includedirs (includeDirList)
        files { "external/graphics-framework/*.cpp" }

    -- Build lodepng static library.
    project "lodepng"
        kind "StaticLib"
        language "C++"
        location "build"
        objdir "build"
        targetdir "lib"
        buildoptions (buildOptions)
        includedirs { includeDirList[1], "external/lodepng" }
        files { "external/lodepng/lodepng.cpp" }

    -- Build the RayTracer application.
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
        files { "src/*.cpp" }

-- #!lua
--
-- includeDirList = {
--     "external",
-- }
--
-- buildOptions = {"-std=c++14"}
--
-- -- Get the current OS platform
-- PLATFORM = os.target()
--
-- -- Create lib directory if it does not exist
-- if not os.isdir("lib") then
--     os.mkdir("lib")
-- end
--
-- -- Build lua-5.4.7 library and copy it into lib if it is not already present.
-- if not os.isfile("lib/liblua.a") then
--     os.chdir("external/lua-5.4.7")
--
--     if PLATFORM == "macosx" then
--         os.execute("make macosx")
--     elseif PLATFORM == "linux" then
--         os.execute("make linux")
--     elseif PLATFORM == "windows" then
--         os.execute("make mingw")
--     end
--
--     os.chdir("../../")
--     os.execute("cp external/lua-5.4.7/src/liblua.a lib/")
-- end
--
--
-- solution "BuildStaticLibs"
--     configurations { "Debug", "Release" }
--
--     filter "configurations:Debug"
--         defines { "DEBUG" }
--         symbols "On"
--
--     filter "configurations:Release"
--         defines { "NDEBUG" }
--         optimize "On"
--
--     -- Builds graphics-framework static library
--     project "graphics-framework"
--         kind "StaticLib"
--         language "C++"
--         location "build"
--         objdir "build"
--         targetdir "lib"
--         buildoptions (buildOptions)
--         includedirs (includeDirList)
--         files { "external/graphics-framework/*.cpp" }
--
--     -- Build lodepng static library
--     project "lodepng"
--         kind "StaticLib"
--         language "C++"
--         location "build"
--         objdir "build"
--         targetdir "lib"
--         includedirs (includeDirList)
--         includedirs {
--             "external/lodepng"
--         }
--         files {
--             "external/lodepng/lodepng.cpp"
--         }
