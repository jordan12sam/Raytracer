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
        void draw(ShaderProgram &shaderProgram, VertexArray &VAO);
    private:
};