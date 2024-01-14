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
#include "Texture.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "Renderer.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Scene.hpp"
#include "Vertex.hpp"
#include "../res/models/Shape.hpp"

#include <iostream>
#include <vector>
#include <chrono>

unsigned int WIDTH = 1920;
unsigned int HEIGHT = 1080;
float AR = (float)WIDTH / (float)HEIGHT;

GLfloat quadVertices[] =
{
	-1.0f, -1.0f,
	-1.0f,  1.0f,
	 1.0f,  1.0f,
	 1.0f, -1.0f
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
        Renderer renderer;

        // Define vertex buffer, vertex array, and index buffer objects
        VertexBufferLayout quadLayout;
        quadLayout.push(GL_FLOAT, 2);
        VertexBuffer quadVBO(quadVertices, 8 * sizeof(GLfloat));
        VertexArray quadVAO;
        quadVAO.addBuffer(quadVBO, quadLayout);
        IndexBuffer quadIBO(quadIndices, 6);

        // Initialise scene
        Scene scene;
        scene.pushCube(glm::vec4(5.0f, 5.0f, 3.0f, 1.0f), glm::vec4(0.8, 0.4, 0.2, 1.0), 2.0f, 0.4f, 0.0f);
        scene.pushCube(glm::vec4(7.5f, 5.0f, -7.0f, 1.0f), glm::vec4(0.2, 0.7, 0.5, 1.0), 2.0f, 0.4f, 0.0f);
        scene.pushCube(glm::vec4(11.0f, 7.0f, 4.0f, 1.0f), glm::vec4(0.7, 0.1, 0.3, 1.0), 6.0f, 0.4f, 0.0f);
        scene.pushCube(glm::vec4(9.0f, 6.0f, -3.0f, 1.0f), glm::vec4(0.5, 0.8, 0.2, 1.0), 4.0f, 0.4f, 0.0f);
        scene.pushCube(glm::vec4(-30.0f, 6.0f, 0.0f, 1.0f), glm::vec4(0.2, 0.8, 0.8, 1.0), 20.0f, 0.8f, 0.0f);
        scene.pushCube(glm::vec4(0.0f, -1001.0f, 0.0f, 1.0f), glm::vec4(1.0, 1.0, 1.0, 1.0), 2000.0f, 0.0f, 0.0f);

        scene.worldSpace.light = glm::vec4(-5.0f, 20.0f, 0.0f, 1.0f);

        //Initialise Texture
        Texture texture("./res/textures/squares.png");
        texture.bind();

        // Define shaders
        Shader vertexShader("./res/shaders/vertexShader.glsl", GL_VERTEX_SHADER);
        Shader fragmentShader("./res/shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER);
        ShaderProgram shaderProgram;
        shaderProgram.attach(vertexShader);
        shaderProgram.attach(fragmentShader);
        shaderProgram.link();
        shaderProgram.bind();

        GLuint ssbo;
        glGenBuffers(1, &ssbo);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

        Camera camera;
        glm::mat4 model(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection(1.0f);

        shaderProgram.setFloat("AR", AR);

        auto begin = std::chrono::high_resolution_clock::now();
        while (window.isOpen())
        {
            auto end = std::chrono::high_resolution_clock::now();
            auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
            auto fps = 10e9 / nanoseconds;
            //std::cout << fps << std::endl;

            double xpos, ypos;
            glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
            xpos = xpos - (float)WIDTH/2.0f;
            ypos = ypos - (float)HEIGHT/2.0f;
            
            camera.processKeyboardInput(nanoseconds);
            camera.processMouseInput(xpos, ypos);

            view = camera.getViewMatrix();

            scene.applyMvp(model, view, projection);
            //std::cout << glm::to_string(scene.cameraSpace.light) << std::endl;

            shaderProgram.bind();
            shaderProgram.setIntArray("indices", &scene.indices[0], (int)scene.indices.size());
            shaderProgram.setInt("numVertices", (int)scene.cameraSpace.vertices.size());
            shaderProgram.setInt("numIndices", (int)scene.indices.size());
            shaderProgram.setVec4("lightPos", scene.cameraSpace.light);

            glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vertex) * (int)scene.cameraSpace.vertices.size(), &scene.cameraSpace.vertices[0], GL_STATIC_DRAW);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

            renderer.draw(shaderProgram, quadVAO);

            begin = std::chrono::high_resolution_clock::now();
        }
    }

}
