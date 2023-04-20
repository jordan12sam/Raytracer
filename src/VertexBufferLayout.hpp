#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <cassert>

struct VertexBufferElement
{
    unsigned int count;
    unsigned int type;
    unsigned char normalised;

    static unsigned int getTypeSize(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 4;
        }
        assert(false);
        return 0;
    }
};

class VertexBufferLayout
{
    private:
        std::vector<VertexBufferElement> elements;
        unsigned int stride;
    public:
        VertexBufferLayout()
            : stride(0) {}

        void push(unsigned int type, unsigned int count)
        {
            elements.push_back({count, type, GL_FALSE});
            stride += count * VertexBufferElement::getTypeSize(type);
        }

        inline const std::vector<VertexBufferElement> getElements() const
        {
            return elements;
        }

        inline unsigned int getStride() const
        {
            return stride;
        }
};