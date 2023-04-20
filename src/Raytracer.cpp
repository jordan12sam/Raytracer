#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Window.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Cube.hpp"

// define screen size
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;
const float AR = (float)SCR_WIDTH/(float)SCR_HEIGHT;

int main()
{
    Window window(SCR_WIDTH, SCR_HEIGHT, "Raytracer");

    Camera camera;

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

        double xpos, ypos;
        glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
        xpos = xpos - (float)SCR_WIDTH/2.0f;
        ypos = ypos - (float)SCR_HEIGHT/2.0f;

        camera.processKeyboardInput(0.001f);
        camera.processMouseInput(xpos, ypos);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(80.0f), AR, 0.1f, 100.0f);
        glm::mat4 mvp = projection * view * model;

        shader.bind();
        shader.setMat4("uMVP", mvp);
        vertexArray.bind();

        glDrawArrays(GL_TRIANGLES, 0, cube.verticesSize/sizeof(float));
    }

    return 0;
}