#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#define GLM_SWIZZLE_XYZW 
#define GLM_ENABLE_EXPERIMENTAL
#include <imgui.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Window.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "Renderer.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Cube.hpp"

#include <iostream>
#include <vector>
#include <chrono>

unsigned int WIDTH = 1600;
unsigned int HEIGHT = 900;
float AR = (float)WIDTH / (float)HEIGHT;

GLfloat quadVertices[] =
{
	-1.0f, -1.0f , 0.0f,    0.0f, 0.0f,
	-1.0f,  1.0f , 0.0f,    0.0f, 1.0f,
	 1.0f,  1.0f , 0.0f,    1.0f, 1.0f,
	 1.0f, -1.0f , 0.0f,    1.0f, 0.0f
};

GLuint quadIndices[] =
{
	0, 2, 1,
	0, 3, 2
};

GLfloat sceneVertices[] =
{
    // Front face
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     1.0, 0.0, 0.0, 1.0,     // Vertex 0
    -0.5f, 0.5f, -0.5f,     0.0f, 1.0f,     0.0, 1.0, 0.0, 1.0,     // Vertex 1
    0.5f, 0.5f, -0.5f,      1.0f, 1.0f,     0.0, 0.0, 1.0, 1.0,     // Vertex 2
    0.5f, -0.5f, -0.5f,     1.0f, 0.0f,     0.0, 0.0, 0.0, 1.0,     // Vertex 3

    // Back face
    -0.5f, -0.5f, 0.5f,     0.0f, 0.0f,     1.0, 0.0, 0.0, 1.0,     // Vertex 4
    -0.5f,  0.5f, 0.5f,     0.0f, 1.0f,     0.0, 1.0, 0.0, 1.0,     // Vertex 5
    0.5f,  0.5f, 0.5f,      1.0f, 1.0f,     0.0, 0.0, 1.0, 1.0,     // Vertex 6
    0.5f, -0.5f, 0.5f,      1.0f, 0.0f,     0.0, 0.0, 0.0, 1.0,     // Vertex 7

    // Left face
    -0.5f, -0.5f, 0.5f,     0.0f, 0.0f,     1.0, 0.0, 0.0, 1.0,     // Vertex 8
    -0.5f,  0.5f, 0.5f,     0.0f, 1.0f,     0.0, 1.0, 0.0, 1.0,     // Vertex 9
    -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0, 0.0, 1.0, 1.0,     // Vertex 10
    -0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     0.0, 0.0, 0.0, 1.0,     // Vertex 11

    // Right face
    0.5f, -0.5f, -0.5f,     0.0f, 0.0f,     1.0, 0.0, 0.0, 1.0,     // Vertex 12
    0.5f,  0.5f, -0.5f,     0.0f, 1.0f,     0.0, 1.0, 0.0, 1.0,     // Vertex 13
    0.5f,  0.5f, 0.5f,      1.0f, 1.0f,     0.0, 0.0, 1.0, 1.0,     // Vertex 14
    0.5f, -0.5f, 0.5f,      1.0f, 0.0f,     0.0, 0.0, 0.0, 1.0,     // Vertex 15

    // Top face
    -0.5f, 0.5f, -0.5f,     0.0f, 0.0f,     1.0, 0.0, 0.0, 1.0,     // Vertex 16
    -0.5f, 0.5f, 0.5f,      0.0f, 1.0f,     0.0, 1.0, 0.0, 1.0,     // Vertex 17
    0.5f,   0.5f, 0.5f,     1.0f, 1.0f,     0.0, 0.0, 1.0, 1.0,     // Vertex 18
    0.5f,   0.5f, -0.5f,    1.0f, 0.0f,     0.0, 0.0, 0.0, 1.0,     // Vertex 19

    // Bottom face
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     1.0, 0.0, 0.0, 1.0,   // Vertex 20
    -0.5f, -0.5f, 0.5f,     0.0f, 1.0f,     0.0, 1.0, 0.0, 1.0,   // Vertex 21
    0.5f,  -0.5f, 0.5f,     1.0f, 1.0f,     0.0, 0.0, 1.0, 1.0,   // Vertex 22
    0.5f,  -0.5f, -0.5f,    1.0f, 0.0f,     0.0, 0.0, 0.0, 1.0    // Vertex 23
};

GLint sceneIndices[] =
{
    // Front face
    0, 1, 2,
    2, 3, 0,

    // Back face
    4, 5, 6,
    6, 7, 4,

    // Left face
    8, 9, 10,
    10, 11, 8,

    // Right face
    12, 13, 14,
    14, 15, 12,

    // Top face
    16, 17, 18,
    18, 19, 16,

    // Bottom face
    20, 21, 22,
    22, 23, 20
};

int main()
{
    {    
        // QUAD SETUP
        // Initialise window
        Window window(WIDTH, HEIGHT, "Raytracer");

        // Initialise Renderer
        Renderer renderer(WIDTH, HEIGHT);

        // Define vertex buffer, vertex array, and index buffer objects
        VertexBufferLayout quadLayout;
        quadLayout.push(GL_FLOAT, 3);
        quadLayout.push(GL_FLOAT, 2);
        VertexBuffer quadVBO(quadVertices, 20 * sizeof(GLfloat));
        VertexArray quadVAO;
        quadVAO.addBuffer(quadVBO, quadLayout);
        IndexBuffer quadIBO(quadIndices, 6);

        // Define shaders
        Shader vertexShader("../res/shaders/vertexShader.glsl", GL_VERTEX_SHADER);
        Shader fragmentShader("../res/shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER);
        ShaderProgram shaderProgram;
        shaderProgram.attach(vertexShader);
        shaderProgram.attach(fragmentShader);
        shaderProgram.link();
        shaderProgram.bind();

        shaderProgram.setFloatArray("vertices", sceneVertices, sizeof(sceneVertices) / sizeof(sceneVertices[0]));
        shaderProgram.setIntArray("indices", sceneIndices, sizeof(sceneIndices) / sizeof(sceneIndices[0]));
        shaderProgram.setInt("numVertices", sizeof(sceneVertices) / sizeof(sceneVertices[0]));
        shaderProgram.setInt("vertexSize", 9);
        shaderProgram.setInt("numIndices", sizeof(sceneIndices) / sizeof(sceneIndices[0]));
        shaderProgram.setFloat("AR", AR);

        Camera camera;
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(45.0f, AR, 1.0f, 100000.0f);
        glm::mat4 mvp = projection * view * model;

        auto begin = std::chrono::high_resolution_clock::now();
        while (window.isOpen())
        {
            auto end = std::chrono::high_resolution_clock::now();
            auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
            auto fps = 1 / nanoseconds / 10e-9;

            double xpos, ypos;
            glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
            xpos = xpos - (float)WIDTH/2.0f;
            ypos = ypos - (float)HEIGHT/2.0f;
            
            camera.processKeyboardInput(nanoseconds);
            camera.processMouseInput(xpos, ypos);

            view = camera.getViewMatrix();
            mvp = projection * view * model;
            shaderProgram.bind();
            shaderProgram.setMat4("MVP", mvp);

            renderer.draw(shaderProgram, quadVAO, WIDTH, HEIGHT);

            begin = std::chrono::high_resolution_clock::now();
        }
    }

}
