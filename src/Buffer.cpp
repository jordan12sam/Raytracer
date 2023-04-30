#include "Buffer.hpp"

Buffer::~Buffer()
{
    glDeleteBuffers(1, &ID);
}

void Buffer::bind() const
{
    glBindBuffer(type, ID);
}

void Buffer::unbind() const
{
    glBindBuffer(type, 0);
}

void Buffer::bindBase(GLuint binding) const
{
    glBindBufferBase(type, binding, ID);
}