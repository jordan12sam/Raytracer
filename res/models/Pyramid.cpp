#include "Shape.hpp"

Pyramid::Pyramid(glm::vec4 position, glm::vec4 colour, float scale, float albedo, float textured)
: Shape(position, colour, scale, albedo, textured)
{
    m_vertices =
    {
        Vertex(m_textured, m_albedo, glm::vec2(0.0f, 0.0f), glm::vec4(-0.5f,  0.0f, -0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(1.0f, 0.0f), glm::vec4( 0.5f,  0.0f, -0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(1.0f, 1.0f), glm::vec4( 0.5f,  0.0f,  0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(0.0f, 1.0f), glm::vec4(-0.5f,  0.0f,  0.5f, 1.0f), m_colour),
        
        Vertex(m_textured, m_albedo, glm::vec2(0.5f, 0.5f), glm::vec4( 0.0f,  1.0f,  0.0f, 1.0f), m_colour)
    };

    m_indices =
    {
        0, 1, 2,
        2, 3, 0,

        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

    m_normals = {
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        0.0, -0.4472135954999579, 0.8944271909999159,
        -0.8944271909999159, -0.4472135954999579, 0.0,
        0.0, -0.4472135954999579, -0.8944271909999159,
        0.8944271909999159, -0.4472135954999579, 0.0
    };

    this->transform();
}