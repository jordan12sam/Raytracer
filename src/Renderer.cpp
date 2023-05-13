#include "Renderer.hpp"

Renderer::Renderer(const unsigned int width, const unsigned int height)
{

}

void Renderer::draw(ShaderProgram &shaderProgram, VertexArray &VAO, const unsigned int screenWidth, const unsigned int screenHeight)
{
    shaderProgram.bind();
    VAO.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}