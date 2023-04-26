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
#include "ShaderProgram.hpp"
#include "Renderer.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Cube.hpp"

#include <vector>

unsigned int screenWidth = 1600;
unsigned int screenHeight = 900;

GLfloat quadVertices[] =
{
	-1.0f, -1.0f , 0.0f, 0.0f, 0.0f,
	-1.0f,  1.0f , 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f , 0.0f, 1.0f, 1.0f,
	 1.0f, -1.0f , 0.0f, 1.0f, 0.0f,
};

GLuint quadIndices[] =
{
	0, 2, 1,
	0, 3, 2
};

int main()
{
    {    
        // Initialise window
        Window window(screenWidth, screenHeight, "Raytracer");

        // Initialise Renderer
        Renderer renderer(screenWidth, screenHeight);

        // Define vertex buffer, vertex array, and index buffer objects
        VertexBufferLayout layout;
        layout.push(GL_FLOAT, 3);
        layout.push(GL_FLOAT, 2);
        VertexBuffer VBO(quadVertices, 20 * sizeof(GLfloat));
        VertexArray VAO;
        VAO.addBuffer(VBO, layout);
        IndexBuffer IBO(quadIndices, 6);

        // Define shaders
        Shader vertexShader("../res/shaders/vertexShader.glsl", GL_VERTEX_SHADER);
        Shader fragmentShader("../res/shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER);
        ShaderProgram shaderProgram;
        shaderProgram.attach(vertexShader);
        shaderProgram.attach(fragmentShader);
        shaderProgram.link();

        Shader computeShader("../res/shaders/computeShader.glsl", GL_COMPUTE_SHADER);
        ShaderProgram computeProgram;
        computeProgram.attach(computeShader);
        computeProgram.link();

        while (window.isOpen())
        {
            //TODO: Move this to Renderer::draw
            computeProgram.bind();
            glMemoryBarrier(GL_ALL_BARRIER_BITS);
            glDispatchCompute(ceil(screenWidth / 8), ceil(screenHeight / 4), 1);
            glMemoryBarrier(GL_ALL_BARRIER_BITS);

            shaderProgram.bind();
            glBindTextureUnit(0, renderer.texture);
            shaderProgram.setInt("screen", 0);
            VAO.bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }

}
