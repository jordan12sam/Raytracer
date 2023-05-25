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
#include "Scene.hpp"
#include "../res/models/Shape.hpp"

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

        // Initialise scene
        Scene scene;
        scene.pushCube(glm::vec3(1.7f, -2.3f, 4.0f), glm::vec4(0.6f, 0.2f, 0.8f, 1.0f), 1.0f, 0.0f);
        scene.pushCube(glm::vec3(-1.8f, 2.9f, -2.1f), glm::vec4(0.1f, 0.7f, 0.4f, 1.0f), 2.0f, 0.0f);
        scene.pushCube(glm::vec3(0.9f, -3.1f, 1.8f), glm::vec4(0.9f, 0.5f, 0.1f, 1.0f), 3.0f, 0.0f);
        scene.pushCube(glm::vec3(0.0f, -35.0f, 0.0f), glm::vec4(0.2f, 0.0f, 0.2f, 1.0f), 60.0f, 0.0f);
        scene.pushPyramid(glm::vec3(-3.8f, 3.7f, -1.5f), glm::vec4(0.3f, 0.9f, 0.2f, 1.0f), 1.0f, 0.0f);
        scene.pushPyramid(glm::vec3(3.0f, 1.2f, 2.3f), glm::vec4(0.8f, 0.4f, 0.6f, 1.0f), 2.0f, 0.0f);
        scene.pushPyramid(glm::vec3(-2.7f, -3.9f, 0.8f), glm::vec4(0.2f, 0.7f, 0.9f, 1.0f), 3.0f, 0.0f);


        // Define shaders
        Shader vertexShader("../res/shaders/vertexShader.glsl", GL_VERTEX_SHADER);
        Shader fragmentShader("../res/shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER);
        ShaderProgram shaderProgram;
        shaderProgram.attach(vertexShader);
        shaderProgram.attach(fragmentShader);
        shaderProgram.link();
        shaderProgram.bind();

        shaderProgram.setFloatArray("vertices", &scene.vertices[0], (int)scene.vertices.size());
        shaderProgram.setIntArray("indices", &scene.indices[0], (int)scene.indices.size());
        shaderProgram.setFloatArray("normals", &scene.normals[0], (int)scene.normals.size());
        shaderProgram.setInt("numVertices", (int)scene.vertices.size());
        shaderProgram.setInt("vertexSize", 10);
        shaderProgram.setInt("numIndices", (int)scene.indices.size());
        shaderProgram.setFloat("AR", AR);

        Camera camera;
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 modelView = view * model;
        glm::mat4 projection = glm::perspective(45.0f, AR, 1.0f, 100000.0f);
        glm::mat4 mvp = projection * modelView;
        glm::mat4 normalMvp = projection * transpose(inverse(modelView));

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
            glm::mat4 normalMvp = projection * transpose(inverse(modelView));

            shaderProgram.bind();
            shaderProgram.setMat4("MVP", mvp);
            shaderProgram.setMat4("normalMVP", normalMvp);

            renderer.draw(shaderProgram, quadVAO, WIDTH, HEIGHT);

            begin = std::chrono::high_resolution_clock::now();
        }
    }

}
