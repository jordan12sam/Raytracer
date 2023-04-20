#include "Buffer.hpp"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    type = getType();
    glGenBuffers(1, &rendererID);
    this->bind();
    glBufferData(type, size, data, GL_STATIC_DRAW);
}

unsigned int VertexBuffer::getType() const
{
    return GL_ARRAY_BUFFER;
}