#include "Renderer.hpp"

void Renderer::draw(ShaderProgram &shaderProgram, VertexArray &VAO)
{
    shaderProgram.bind();
    VAO.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}