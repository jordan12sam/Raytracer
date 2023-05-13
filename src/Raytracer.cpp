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
// Vertex Positions     // Texture      // Colour
-0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     1.0, 0.0, 0.0, 1.0,
-0.5f, 0.5f, -0.5f,     0.0f, 1.0f,     0.0, 1.0, 0.0, 1.0,
0.5f, 0.5f, -0.5f,      1.0f, 1.0f,     0.0, 0.0, 1.0, 1.0,
0.5f, -0.5f, -0.5f,     1.0f, 0.0f,     0.0, 0.0, 0.0, 1.0,
-0.5f, -0.5f, 0.5f,     0.0f, 0.0f,     1.0, 0.0, 0.0, 1.0,
-0.5f, 0.5f, 0.5f,      0.0f, 1.0f,     0.0, 1.0, 0.0, 1.0,
0.5f, 0.5f, 0.5f,       1.0f, 1.0f,     0.0, 0.0, 1.0, 1.0,
0.5f, -0.5f, 0.5f,      1.0f, 0.0f,     0.0, 0.0, 0.0, 1.0
};

GLint sceneIndices[] =
{
// Front face
0, 1, 2,
0, 2, 3,
// Back face
4, 6, 5,
4, 7, 6,
// Left face
4, 5, 1,
4, 1, 0,
// Right face
3, 2, 6,
3, 6, 7,
// Top face
1, 5, 6,
1, 6, 2,
// Bottom face
4, 0, 3,
4, 3, 7
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
            //camera.processMouseInput(xpos, ypos);

            view = camera.getViewMatrix();
            mvp = projection * view * model;
            shaderProgram.bind();
            shaderProgram.setMat4("MVP", mvp);

            renderer.draw(shaderProgram, quadVAO, WIDTH, HEIGHT);

            begin = std::chrono::high_resolution_clock::now();
        }
    }

}
