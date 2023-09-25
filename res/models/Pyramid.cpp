#include "Shape.hpp"

Pyramid::Pyramid(glm::vec3 position, glm::vec4 colour, float scale, float albedo, float textured)
: Shape(position, colour, scale, albedo, textured)
{
    m_vertices =
    {
        // Position            //Texture   //Colour                                            //Albedo    //Textured
        -0.5f,  0.0f, -0.5f,   0.0f, 0.0f,   m_colour.x, m_colour.y, m_colour.z, m_colour.w,    m_albedo,   m_textured,
         0.5f,  0.0f, -0.5f,   1.0f, 0.0f,   m_colour.x, m_colour.y, m_colour.z, m_colour.w,    m_albedo,   m_textured,
         0.5f,  0.0f,  0.5f,   1.0f, 1.0f,   m_colour.x, m_colour.y, m_colour.z, m_colour.w,    m_albedo,   m_textured,
        -0.5f,  0.0f,  0.5f,   0.0f, 1.0f,   m_colour.x, m_colour.y, m_colour.z, m_colour.w,    m_albedo,   m_textured,

         0.0f,  1.0f,  0.0f,   0.5f, 0.5f,   m_colour.x, m_colour.y, m_colour.z, m_colour.w,    m_albedo,   m_textured
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