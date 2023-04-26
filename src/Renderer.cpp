#include "Renderer.hpp"

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    va.bind();
    ib.bind();

    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}