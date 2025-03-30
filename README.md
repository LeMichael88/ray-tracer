# Custom Ray Tracer Project

**Author:** Michael Le

----

## Overview
This is a custom CPU-based ray tracer written in C++.
It is designed to be a simple and efficient ray tracing engine that can render 3D scenes with realistic lighting
and shading effects. Implemented features include:

* Basic ray tracing functionality
  * Recursive ray tracing
  * Ray-sphere, ray-plane, and ray-triangle intersection testing
  * Phong illumination model (ambient, diffuse, specular lighting)
  * Shadows and reflections
* Advanced rendering techniques
  * Anti-aliasing
  * Texture mapping
  * Displacement mapping
* Custom Lua scene configuration
  * Define a scene graph using objects, lights, and materials 
  * Hierarchical transformations (translation, rotation, scaling)
  * Animation support
  * Particle system
* Optimizations
  * Multi-threaded rendering for performance optimization

---

## Prerequisites
* C++17 or higher
* CMake
* Premake
  * https://premake.github.io/

## Dependencies
This project uses the following libraries, found in the `external` folder:
* Lua
  * http://www.lua.org/
* GLM
  * http://glm.g-truc.net/0.9.7/index.html
* ImGui
  * https://github.com/ocornut/imgui
* Graphics Framework 
  * Helper functions for reading mesh files, from CS488 course at University of Waterloo
* Polyroots helper functions (found directly in `src` folder)
  * Helper functions for finding quadratic roots, from University of Washington, James Painter

---

## Build Instructions
**Premake5** is used as a cross-platform build system.
First you will need to build all the static libraries that the projects depend on.
To build the libraries, run the following in the root directory:
```
premake5 gmake
make
```

This will build the following libraries, and place them in the top level **lib** folder:
* liblua.a
* libgraphics-framework.a
* libimgui.a

----

## Usage
You can find sample configuration files in the `tests` folder.
These files use the mesh files and texture images found in the `assets` folder.
Sample animation videos are also included in the `renders` folder.

To run the ray tracer, you will need to create a configuration file in Lua.
The configuration file is a Lua script that defines the scene to be rendered.
After creating the file, run the following command from the root directory:
```
./RayTracer <path-to-lua-script>
```

For example, to run the ray tracer with the provided configuration file, run:
```
./RayTracer tests/nonhier.lua 
```

This generates the image in the specified output folder of the lua script.

----

## Using the Animation System

The program will generate individual frames, which can be combined into a video using `ffmpeg`.
Use the following command to combine the frames into a video:
```
ffmpeg -r 24 -i animation_%4d.png -c:v libx264 -vf fps=24 -pix_fmt yuv420p animation.mp4
```