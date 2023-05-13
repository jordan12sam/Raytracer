cmake_minimum_required(VERSION 3.12)

project(Raytracer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Use FetchContent to download GLAD library
include(FetchContent)
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.34
)
FetchContent_GetProperties(glad)
FetchContent_Populate(glad)
add_subdirectory(${glad_SOURCE_DIR} ${glad_BINARY_DIR})

# Use FetchContent to download GLFW library
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.4
)
FetchContent_GetProperties(glfw)
FetchContent_Populate(glfw)
add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})

# Use FetchContent to download GLM library
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
FetchContent_GetProperties(glm)
FetchContent_Populate(glm)
add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})

# Download STB_IMAGE
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
)
FetchContent_GetProperties(stb)
FetchContent_Populate(stb)

# Build imgui library
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.60
)
FetchContent_GetProperties(imgui)
FetchContent_Populate(imgui)
add_library(IMGUI)
target_sources(IMGUI 
    PRIVATE
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
    PRIVATE
        ${imgui_SOURCE_DIR}/examples/opengl3_example/imgui_impl_glfw_gl3.cpp)
target_include_directories(IMGUI
    PUBLIC  ${imgui_SOURCE_DIR}
    PUBLIC  ${imgui_SOURCE_DIR}/backends)
target_link_libraries(IMGUI
    PRIVATE glad
    PRIVATE glfw)


# Add the executable
file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.hpp"
        "${PROJECT_SOURCE_DIR}/res/models/*.hpp"
        "${PROJECT_SOURCE_DIR}/res/models/*.cpp"
        )
add_executable(Raytracer ${all_SRCS})

# Link the executable to libraries
target_link_libraries(Raytracer 
    PRIVATE glad
    PRIVATE glfw
    PRIVATE glm
    PRIVATE IMGUI)

target_include_directories(Raytracer
    PRIVATE ${glad_SOURCE_DIR}/include
    PRIVATE ${glfw_SOURCE_DIR}/include
    PRIVATE ${glm_SOURCE_DIR}
    PRIVATE ${stb_SOURCE_DIR})

add_dependencies(Raytracer 
    glad
    glfw
    glm)
