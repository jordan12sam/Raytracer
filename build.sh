#!/bin/bash

mkdir build
cd build
cmake ..
sed -i 's/<GL\/gl3w.h>/<glad\/glad.h>/g' ./_deps/imgui-src/examples/opengl3_example/imgui_impl_glfw_gl3.cpp
cmake --build .