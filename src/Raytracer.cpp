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
#include "Renderer.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Cube.hpp"

#include <vector>

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

    std::vector<float> vertices;
    std::vector<int> indices;

    Renderer renderer;

    for (int i = 0; i < 120; i += 5) {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));

        glm::vec4 position(cube.vertices[i], cube.vertices[i + 1], cube.vertices[i + 2], 1.0f);
        glm::vec2 texture(cube.vertices[i + 3], cube.vertices[i + 4]);

        position = transform * position;

        vertices.push_back(position.x);
        vertices.push_back(position.y);
        vertices.push_back(position.z);
        vertices.push_back(texture.x);
        vertices.push_back(texture.y);
    }

    for (int i = 0; i < 36; i++)
    {
        indices.push_back(cube.indices[i]);
    }

    std::cout << indices.size() << " " << vertices.size() << std::endl;

    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 3);
    layout.push(GL_FLOAT, 2);

    VertexBuffer vertexBuffer(vertices, vertices.size() * sizeof(GLfloat));
    VertexArray vertexArray;
    vertexArray.addBuffer(vertexBuffer, layout);
    IndexBuffer indexBuffer(indices, indices.size());

    vertexBuffer.unbind();
    indexBuffer.unbind();
    vertexArray.unbind();

    glEnable(GL_DEPTH_TEST);

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
        shader.setMat4("uMVP", mvp);

        renderer.draw(vertexArray, indexBuffer, shader);
    }

    return 0;
}