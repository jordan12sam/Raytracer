#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.hpp"
#include "Buffer.hpp"
#include "Shader.hpp"

#include <iostream>

class Renderer
{
    private:

    public:
        void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        void clear() const;
};