#include "Shape.hpp"

Shape::~Shape()
{
    m_vertices = std::vector<GLfloat>();
    m_indices = std::vector<GLint>();
}

void Shape::push(std::vector<GLfloat> &sceneVertices, std::vector<GLint> &sceneIndices)
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

    for (int i = 0; i < m_vertices.size(); i++)
    {
        sceneVertices.push_back(m_vertices[i]);
    }
}

void Shape::transform()
{
    for (int i = 0; i < m_vertices.size(); i += 9)
    {
        glm::vec3 positions = glm::vec3(m_vertices[i], m_vertices[i + 1], m_vertices[i + 2]);
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_position);
        glm::mat4 scaler = glm::scale(glm::mat4(1.0f), glm::vec3(m_scale));
        glm::mat4 transformation = translation * scaler;
        glm::vec3 newPositions = transformation * glm::vec4(positions, 1.0f);
        m_vertices[i] = newPositions.x;
        m_vertices[i + 1] = newPositions.y;
        m_vertices[i + 2] = newPositions.z;
    }
}