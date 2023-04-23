#include "Buffer.hpp"

ShaderStorageBuffer::ShaderStorageBuffer(const std::vector<float> data, unsigned int size)
{
    type = getType();
    glGenBuffers(1, &rendererID);
    this->bind();
    glBufferData(type, size, &data[0], GL_STATIC_DRAW);
}

unsigned int ShaderStorageBuffer::getType() const
{
    return GL_SHADER_STORAGE_BUFFER;
}