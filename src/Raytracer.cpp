#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Window.hpp"
#include "Shader.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Cube.hpp"

int main()
{
    Window window(1600, 900, "Raytracer");

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

    while (window.isOpen())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        shader.bind();
        vertexArray.bind();

        glDrawArrays(GL_TRIANGLES, 0, cube.verticesSize/sizeof(float));
    }

    return 0;
}