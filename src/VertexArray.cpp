#include "VertexArray.hpp"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &rendererID);
    glBindVertexArray(rendererID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &rendererID);
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    bind();
    vb.bind();
    const auto& elements = layout.getElements();
    intptr_t offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.getStride(), (const void*)offset);
        offset += element.count * VertexBufferElement::getTypeSize(element.type);
    }
}

void VertexArray::bind() const
{
    glBindVertexArray(rendererID);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}