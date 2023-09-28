#include "Shape.hpp"

Cube::Cube(glm::vec4 position, glm::vec4 colour, float scale, float albedo, float textured)
: Shape(position, colour, scale, albedo, textured)
{
    m_vertices =
    {
        Vertex(m_textured, m_albedo, glm::vec2(0.0f, 0.0f), glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(1.0f, 0.0f), glm::vec4( 0.5f, -0.5f,  0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(1.0f, 1.0f), glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(0.0f, 1.0f), glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f), m_colour),

        Vertex(m_textured, m_albedo, glm::vec2(0.0f, 0.0f), glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(1.0f, 0.0f), glm::vec4( 0.5f, -0.5f, -0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(1.0f, 1.0f), glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(0.0f, 1.0f), glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f), m_colour),

        Vertex(m_textured, m_albedo, glm::vec2(0.0f, 0.0f), glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(1.0f, 0.0f), glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(1.0f, 1.0f), glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(0.0f, 1.0f), glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f), m_colour),

        Vertex(m_textured, m_albedo, glm::vec2(0.0f, 0.0f), glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(1.0f, 0.0f), glm::vec4( 0.5f, -0.5f, -0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(1.0f, 1.0f), glm::vec4( 0.5f, -0.5f,  0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(0.0f, 1.0f), glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f), m_colour),
        
        Vertex(m_textured, m_albedo, glm::vec2(0.0f, 0.0f), glm::vec4( 0.5f, -0.5f, -0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(1.0f, 0.0f), glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(1.0f, 1.0f), glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(0.0f, 1.0f), glm::vec4( 0.5f, -0.5f,  0.5f, 1.0f), m_colour),

        Vertex(m_textured, m_albedo, glm::vec2(0.0f, 0.0f), glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(1.0f, 0.0f), glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(1.0f, 1.0f), glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f), m_colour),
        Vertex(m_textured, m_albedo, glm::vec2(0.0f, 1.0f), glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f), m_colour)
    };

    m_indices =
    {
        0, 1, 2,
        2, 3, 0,

        4, 5, 6,
        6, 7, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    m_normals = {
        0.0, 0.0, -1.0,
        0.0, 0.0, -1.0,

        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,

        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0,

        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,

        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,

        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0
    };
    
    this->transform();
}
