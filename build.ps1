mkdir build
cd build
cmake ..
(Get-Content ./_deps/imgui-src/examples/opengl3_example/imgui_impl_glfw_gl3.cpp) | ForEach-Object { $_ -replace "<GL/gl3w.h>", "<glad/glad.h>" } | Set-Content ./_deps/imgui-src/examples/opengl3_example/imgui_impl_glfw_gl3.cpp
cmake --build .