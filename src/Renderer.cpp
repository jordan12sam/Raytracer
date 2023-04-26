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
    computeProgram.bind();
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    glDispatchCompute(ceil(width / 8), ceil(height / 4), 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    shaderProgram.bind();
    glBindTextureUnit(0, texture);
    shaderProgram.setInt("screen", 0);
    VAO.bind();
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}