#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.hpp"
#include "Buffer.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"

#include <iostream>

class Renderer
{
    public:
        Renderer(const unsigned int width, const unsigned int height);
        void draw(ShaderProgram &computeProgram, ShaderProgram &shaderProgram, VertexArray &VAO, const unsigned int width, const unsigned int height);
        GLuint texture;
    private:
};