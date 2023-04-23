#include "Buffer.hpp"

ShaderStorageBuffer::ShaderStorageBuffer(unsigned int size)
{
    type = getType();
    glGenBuffers(1, &rendererID);
    this->bind(0);
    glBufferData(type, size, nullptr, GL_DYNAMIC_COPY);
}

unsigned int ShaderStorageBuffer::getType() const
{
    return GL_SHADER_STORAGE_BUFFER;
}