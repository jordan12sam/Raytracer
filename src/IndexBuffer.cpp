#include "Buffer.hpp"

IndexBuffer::IndexBuffer(const GLuint* data, unsigned int count)
    : count(count)
{
    type = getType();
    glGenBuffers(1, &rendererID);
    this->bind();
    glBufferData(type, count * sizeof(unsigned int), &data[0], GL_STATIC_DRAW);
}

unsigned int IndexBuffer::getType() const
{
    return GL_ELEMENT_ARRAY_BUFFER;
}

unsigned int IndexBuffer::getCount() const
{
    return count;
}