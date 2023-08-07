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
        VertexBuffer quadVBO(quadVertices, 20 * sizeof(GLfloat));
        VertexArray quadVAO;
        quadVAO.addBuffer(quadVBO, quadLayout);
        IndexBuffer quadIBO(quadIndices, 6);

        // Initialise scene
        Scene scene;
        scene.pushCube(glm::vec3(0.0f, 0.0f, -8.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0.0f);


        // Define shaders
        Shader vertexShader("../res/shaders/vertexShader.glsl", GL_VERTEX_SHADER);
        Shader fragmentShader("../res/shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER);
        ShaderProgram shaderProgram;
        shaderProgram.attach(vertexShader);
        shaderProgram.attach(fragmentShader);
        shaderProgram.link();
        shaderProgram.bind();

        Camera camera;
        glm::mat4 view = camera.getViewMatrix();
        float nearPlane = 1.0f;
        float farPlane = 100.0f;
        float fov = 90.0f;
        glm::mat4 projection = glm::perspective(fov, AR, nearPlane, farPlane);

        shaderProgram.setFloat("AR", AR);
        shaderProgram.setFloat("near", nearPlane);
        shaderProgram.setFloat("far", farPlane);

        auto begin = std::chrono::high_resolution_clock::now();
        while (window.isOpen())
        {
            auto end = std::chrono::high_resolution_clock::now();
            auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
            auto fps = 1 / nanoseconds / 10e-9;
            //std::cout << nanoseconds << std::endl;

            double xpos, ypos;
            glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
            xpos = xpos - (float)WIDTH/2.0f;
            ypos = ypos - (float)HEIGHT/2.0f;
            
            camera.processKeyboardInput(nanoseconds);
            camera.processMouseInput(xpos, ypos);

            view = camera.getViewMatrix();
            glm::mat4 projectionView = projection * view;
            glm::mat4 inverseProjectionView = glm::inverse(projectionView);
            //std::cout << glm::to_string(camera.getPosition()) << std::endl;

            shaderProgram.bind();
            shaderProgram.setMat4("inverseProjectionView", inverseProjectionView);
            shaderProgram.setVec3("cameraPosition", camera.getPosition());
            shaderProgram.setFloatArray("vertices", &scene.vertices[0], (int)scene.vertices.size());
            shaderProgram.setIntArray("indices", &scene.indices[0], (int)scene.indices.size());
            shaderProgram.setFloatArray("normals", &scene.normals[0], (int)scene.normals.size());
            shaderProgram.setInt("numVertices", (int)scene.vertices.size());
            shaderProgram.setInt("vertexSize", 10);
            shaderProgram.setInt("numIndices", (int)scene.indices.size());

            renderer.draw(shaderProgram, quadVAO);

            begin = std::chrono::high_resolution_clock::now();
        }
    }

}
