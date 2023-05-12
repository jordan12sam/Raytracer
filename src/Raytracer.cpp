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

#include <vector>

unsigned int screenWidth = 1600;
unsigned int screenHeight = 900;

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
	-0.5f, -0.5f, 3.0f,    0.0f, 0.0f,     1.0, 0.0, 0.0, 1.0,
	-0.5f,  0.5f, 3.0f,    0.0f, 0.5f,     0.0, 1.0, 0.0, 1.0,
	 0.5f,  0.5f, 3.0f,    0.5f, 0.5f,     1.0, 0.0, 1.0, 1.0,
	 0.5f, -0.5f, 3.0f,    0.5f, 0.0f,     1.0, 1.0, 1.0, 1.0
};

GLint sceneIndices[] =
{
	0, 2, 1,
	0, 3, 2
};

int main()
{
    {    
        // QUAD SETUP
        // Initialise window
        Window window(screenWidth, screenHeight, "Raytracer");

        // Initialise Renderer
        Renderer renderer(screenWidth, screenHeight);

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

        Shader computeShader("../res/shaders/computeShader.glsl", GL_COMPUTE_SHADER);
        ShaderProgram computeProgram;
        computeProgram.attach(computeShader);
        computeProgram.link();

        // SCENE SETUP
        computeProgram.bind();
        computeProgram.setFloatArray("vertices", sceneVertices, sizeof(sceneVertices) / sizeof(sceneVertices[0]));
        computeProgram.setIntArray("indices", sceneIndices, sizeof(sceneIndices) / sizeof(sceneIndices[0]));
        computeProgram.setInt("numVertices", sizeof(sceneVertices) / sizeof(sceneVertices[0]));
        computeProgram.setInt("vertexSize", 9);
        computeProgram.setInt("numIndices", sizeof(sceneIndices) / sizeof(sceneIndices[0]));

        Camera camera;
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(80.0f), (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        glm::mat4 mvp = projection * view * model;
        glm::vec3 test = glm::vec4(-0.5f, -0.5f , 0.0f, 1.0f) * mvp;
        std::cout << glm::to_string(test) << std::endl;

        computeProgram.setMat4("mvp", glm::mat4(1.0f));

        while (window.isOpen())
        {
            renderer.draw(computeProgram, shaderProgram, quadVAO, screenWidth, screenHeight);
        }
    }

}
