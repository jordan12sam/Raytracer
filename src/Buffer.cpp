#include "Buffer.hpp"

Buffer::~Buffer()
{
    glDeleteBuffers(1, &rendererID);
}

void Buffer::bind() const
{
    glBindBuffer(type, rendererID);
}

void Buffer::unbind() const
{
    glBindBuffer(type, 0);
}