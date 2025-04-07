cmake_minimum_required(VERSION 3.10)
project(RayTracer CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set optimization flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
    add_definitions(-DNDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/external)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/core)
include_directories(${CMAKE_SOURCE_DIR}/src/geometry)
include_directories(${CMAKE_SOURCE_DIR}/src/particles)
include_directories(${CMAKE_SOURCE_DIR}/src/materials)
include_directories(${CMAKE_SOURCE_DIR}/src/lighting)
include_directories(${CMAKE_SOURCE_DIR}/src/animation)
include_directories(${CMAKE_SOURCE_DIR}/src/utils)

# Create build/lib directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Handle Lua dependency
set(LUA_LIB ${CMAKE_BINARY_DIR}/lib/liblua.a)
if(NOT EXISTS ${LUA_LIB})
    message(STATUS "Building Lua library...")
    
    if(APPLE)
        execute_process(
            COMMAND make macosx
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/external/lua-5.4.7
        )
    elseif(UNIX)
        execute_process(
            COMMAND make linux
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/external/lua-5.4.7
        )
    elseif(WIN32)
        execute_process(
            COMMAND make mingw
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/external/lua-5.4.7
        )
    endif()
    
    file(COPY ${CMAKE_SOURCE_DIR}/external/lua-5.4.7/src/liblua.a
         DESTINATION ${CMAKE_BINARY_DIR}/lib/)
endif()

# Add graphics-framework static library
add_library(graphics-framework STATIC)
file(GLOB GRAPHICS_FRAMEWORK_SOURCES "${CMAKE_SOURCE_DIR}/external/graphics-framework/*.cpp")
target_sources(graphics-framework PRIVATE
    ${GRAPHICS_FRAMEWORK_SOURCES}
)

# Add lodepng static library
add_library(lodepng STATIC)
file(GLOB LODEPNG_SOURCES "${CMAKE_SOURCE_DIR}/external/lodepng/lodepng.cpp")
target_sources(lodepng PRIVATE
    ${LODEPNG_SOURCES}
)
target_include_directories(lodepng PRIVATE
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/external/lodepng
)

# Add the RayTracer executable
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/**/*.cpp")
add_executable(RayTracer ${SOURCES})

# Link libraries
target_link_libraries(RayTracer
    graphics-framework
    ${LUA_LIB}
    lodepng
)

# Platform-specific linking
if(UNIX AND NOT APPLE)
    target_link_libraries(RayTracer stdc++ dl pthread)
endif()
