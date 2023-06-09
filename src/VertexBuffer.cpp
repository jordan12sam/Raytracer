#include "Buffer.hpp"

VertexBuffer::VertexBuffer(const GLfloat* data, unsigned int size)
{
    type = getType();
    glGenBuffers(1, &ID);
    this->bind();
    glBufferData(type, size, &data[0], GL_STATIC_DRAW);
}

unsigned int VertexBuffer::getType() const
{
    return GL_ARRAY_BUFFER;
}