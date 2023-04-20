#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#include "Shader.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Cube.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1600, 900, "My Window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 1980, 1080);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader shader("../res/vertexShader.glsl", "../res/fragmentShader.glsl");

    Cube cube;

    VertexBuffer vertexBuffer(cube.vertices, cube.verticesSize);

    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 3);

    VertexArray vertexArray;
    vertexArray.addBuffer(vertexBuffer, layout);

    IndexBuffer indexBuffer(cube.indices, cube.indicesCount);

    vertexBuffer.unbind();
    indexBuffer.unbind();
    vertexArray.unbind();

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.bind();
        vertexArray.bind();

        glDrawArrays(GL_TRIANGLES, 0, cube.verticesSize/sizeof(float));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}