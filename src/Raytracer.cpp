#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Window.hpp"
#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Cube.hpp"

#include <vector>

const unsigned int SCREEN_WIDTH = 1600;
const unsigned int SCREEN_HEIGHT = 900;

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


const char* screenVertexShaderSource = R"(#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uvs;
out vec2 UVs;
void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
	UVs = uvs;
})";
const char* screenFragmentShaderSource = R"(#version 460 core
out vec4 FragColor;
uniform sampler2D screen;
in vec2 UVs;
void main()
{
	FragColor = texture(screen, UVs);
})";
const char* screenComputeShaderSource = R"(#version 460 core
layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D screen;
void main()
{
    // Scale screen coordinates to [0.0, 1.0]
	ivec2 pixelCoordinates = ivec2(gl_GlobalInvocationID.xy);
	ivec2 dimensions = imageSize(screen);
	float x = (float(pixelCoordinates.x) / dimensions.x);
	float y = (float(pixelCoordinates.y) / dimensions.y);

    vec4 pixelColour = vec4(x, y, 0.0, 1.0);

	imageStore(screen, pixelCoordinates, pixelColour);
})";


int main()
{
    {    
        // Initialise window
        Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "Raytracer");

        // Define vertex buffer, vertex array, and index buffer objects
        VertexBufferLayout layout;
        layout.push(GL_FLOAT, 3);
        layout.push(GL_FLOAT, 2);
        VertexBuffer VBO(quadVertices, 20 * sizeof(GLfloat));
        VertexArray VAO;
        VAO.addBuffer(VBO, layout);
        IndexBuffer IBO(quadIndices, 6);

        // Define texture to be rendered to the screen
        GLuint screenTex;
        glCreateTextures(GL_TEXTURE_2D, 1, &screenTex);
        glTextureParameteri(screenTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(screenTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(screenTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(screenTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureStorage2D(screenTex, 1, GL_RGBA32F, SCREEN_WIDTH, SCREEN_HEIGHT);
        glBindImageTexture(0, screenTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

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
            computeProgram.bind();
            glDispatchCompute(ceil(SCREEN_WIDTH / 8), ceil(SCREEN_HEIGHT / 4), 1);
            glMemoryBarrier(GL_ALL_BARRIER_BITS);

            shaderProgram.bind();
            glBindTextureUnit(0, screenTex);
            shaderProgram.setInt("screen", 0);
            VAO.bind();
            glDrawElements(GL_TRIANGLES, sizeof(quadIndices) / sizeof(quadIndices[0]), GL_UNSIGNED_INT, 0);
        }
    }
}
