#include "Renderer.hpp"

Renderer::Renderer(const unsigned int width, const unsigned int height)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(texture, 1, GL_RGBA32F, width, height);
    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

void Renderer::draw(ShaderProgram computeProgram, ShaderProgram shaderProgram, VertexArray VAO, const unsigned int width, const unsigned int height)
{
    //TODO: Why does the render flicker from within here?
}