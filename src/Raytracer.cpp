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

    //Generate scene vertices
    std::vector<float> vertices;
    std::vector<int> indices;
    Cube cube;
    cube.spawn(vertices, indices, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));
    cube.spawn(vertices, indices, glm::vec3(0.2f, 1.0f, -3.5f), glm::vec3(2.8f), glm::vec4(0.2f, 0.5f, 1.0f, 1.0f));
    cube.spawn(vertices, indices, glm::vec3(4.1f, 0.1f, 3.0f), glm::vec3(1.2f), glm::vec4(0.5f, 1.0f, 0.2f, 1.0f));
    cube.spawn(vertices, indices, glm::vec3(-2.3f, 0.7f, 1.8f), glm::vec3(0.8f), glm::vec4(0.8f, 0.4f, 0.6f, 1.0f));
    cube.spawn(vertices, indices, glm::vec3(0.0f, -32.0f, 0.0f), glm::vec3(60.0f), glm::vec4(0.2f, 1.0f, 0.8f, 1.0f));
    cube.spawn(vertices, indices, glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    std::vector<float> dataIn(SCR_WIDTH * SCR_HEIGHT * 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, dataIn.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    ComputeShader computeShader;
    computeShader.createShader("../res/computeShader.glsl");
    computeShader.setVec3("cameraPos", glm::vec3(0.0f, 0.0f, 0.0f));
    computeShader.setVec2("screenSize", glm::vec2(SCR_WIDTH, SCR_HEIGHT));

    computeShader.bind();
    computeShader.compute(SCR_WIDTH, SCR_HEIGHT, 1);

    int width, height;
    std::vector<float> data;
    glBindImageTexture(2, textureID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    data.resize(width * height * 4); // 4 components for rgba32f format
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, data.data());
    glBindImageTexture(2, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

    std::cout << data[0] << std::endl;

    GLfloat quadVertices[] = {
        // Positions         // Texture Coords
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
        1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,  1.0f, 1.0f
    };

    GLuint quadIndices[] = {
        0, 1, 3,
        1, 2, 3
    };

    GLuint quadVAO, quadVBO, quadEBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    Shader shader;
    shader.createShader("../res/vertexShader.glsl", "../res/fragmentShader.glsl");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    shader.setInt("textureSampler", 0);

    shader.bind();
    glBindVertexArray(quadVAO);
    glBindVertexArray(quadEBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    while (window.isOpen())
    {
        //MVP Calculations
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(80.0f), AR, 0.1f, 100.0f);
        glm::mat4 mvp = projection * view * model;

        //RENDER HERE

        //User Input
        double xpos, ypos;
        glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
        xpos = xpos - (float)SCR_WIDTH/2.0f;
        ypos = ypos - (float)SCR_HEIGHT/2.0f;

        camera.processMouseInput(xpos, ypos);
        camera.processKeyboardInput(0.01f);
    }

    return 0;
}