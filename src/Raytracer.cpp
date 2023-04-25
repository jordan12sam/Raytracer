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

    cube.spawn(vertices, indices, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));
    cube.spawn(vertices, indices, glm::vec3(0.2f, 1.0f, -3.5f), glm::vec3(2.8f), glm::vec4(0.2f, 0.5f, 1.0f, 1.0f));
    cube.spawn(vertices, indices, glm::vec3(4.1f, 0.1f, 3.0f), glm::vec3(1.2f), glm::vec4(0.5f, 1.0f, 0.2f, 1.0f));
    cube.spawn(vertices, indices, glm::vec3(-2.3f, 0.7f, 1.8f), glm::vec3(0.8f), glm::vec4(0.8f, 0.4f, 0.6f, 1.0f));
    cube.spawn(vertices, indices, glm::vec3(0.0f, -32.0f, 0.0f), glm::vec3(60.0f), glm::vec4(0.2f, 1.0f, 0.8f, 1.0f));
    cube.spawn(vertices, indices, glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));


    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 3);
    layout.push(GL_FLOAT, 2);
    layout.push(GL_FLOAT, 4);

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

        ImGui_ImplGlfwGL3_NewFrame();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(80.0f), AR, 0.1f, 100.0f);
        glm::mat4 mvp = projection * view * model;
        shader.setMat4("uMVP", mvp);

        renderer.draw(vertexArray, indexBuffer, shader);

        double xpos, ypos;
        glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
        xpos = xpos - (float)SCR_WIDTH/2.0f;
        ypos = ypos - (float)SCR_HEIGHT/2.0f;

        camera.processMouseInput(xpos, ypos);
        camera.processKeyboardInput((ImGui::GetIO().Framerate));

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
    }

    return 0;
}