#include "Shape.hpp"

Shape::~Shape()
{
    m_vertices = std::vector<Vertex>();
    m_indices = std::vector<GLint>();
}

void Shape::push(std::vector<Vertex> &sceneVertices, std::vector<GLint> &sceneIndices, std::vector<GLfloat> &sceneNormals)
{
    int numIndices;
    if (sceneIndices.size() > 0)
    {
        numIndices = *std::max_element(sceneIndices.begin(), sceneIndices.end()) + 1;
    }
    else
    {
        numIndices = 0;
    }

    for (int i = 0; i < m_indices.size(); i++)
    {
        sceneIndices.push_back(m_indices[i] + numIndices);
    }

    for (int i = 0; i < m_normals.size(); i ++)
    {
        sceneNormals.push_back(m_normals[i]);
    }

        for (int i = 0; i < m_vertices.size(); i++)
    {
        sceneVertices.push_back(m_vertices[i]);
    }
}

void Shape::transform()
{
    for (int i = 0; i < m_vertices.size(); i ++)
    {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(m_position));
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(m_scale));
        glm::mat4 transformation = translation * scale;
        m_vertices[i].position = transformation * m_vertices[i].position;
    }
}