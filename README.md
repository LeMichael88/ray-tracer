# Ray Tracer

**Author:** Michael Le

This is a ray tracer written in C++ using OpenGL and GLM.
It is designed to be a simple and efficient ray tracing engine that can render 3D scenes with realistic lighting
and shading effects.
Some additional features include a particle system and basic animation support.

---

## Dependencies
* Lua
    * http://www.lua.org/
* Premake
    * https://premake.github.io/
* GLM
    * http://glm.g-truc.net/0.9.7/index.html
* ImGui
    * https://github.com/ocornut/imgui
* Polyroots helper: University of Washington, Author:  James Painter
    * https://www.cs.washington.edu/education/courses/cse457/03au/projects/polyroots/
* Graphics Framework - University of Waterloo

---

## Building 
We use **premake5** as our cross-platform build system.
First you will need to build all the static libraries that the projects depend on.
To build the libraries, run the following in the root directory:
```
premake5 gmake
make
```

This will build the following static libraries, and place them in the top level **lib** folder:
* libgraphics-framework.a
* libimgui.a

----

## Using the Animation System

This ray tracer will generate individual frames.
To combine frames into a mp4 file, you can use `ffmpeg` with the following command:
```
ffmpeg -r 24 -i animation_%4d.png -c:v libx264 -vf fps=24 -pix_fmt yuv420p animation.mp4
```