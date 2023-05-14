#include "Shape.hpp"

Shape Shape::setPyramid()
{
    m_vertices =
    {
        // Position            // Texture   //Colour                                            //Albedo
        -0.5f,  0.0f, -0.5f,   0.0f, 0.0f,   m_colour.x, m_colour.y, m_colour.z, m_colour.w,    m_albedo,
         0.5f,  0.0f, -0.5f,   1.0f, 0.0f,   m_colour.x, m_colour.y, m_colour.z, m_colour.w,    m_albedo,
         0.5f,  0.0f,  0.5f,   1.0f, 1.0f,   m_colour.x, m_colour.y, m_colour.z, m_colour.w,    m_albedo,
        -0.5f,  0.0f,  0.5f,   0.0f, 1.0f,   m_colour.x, m_colour.y, m_colour.z, m_colour.w,    m_albedo,

         0.0f,  1.0f,  0.0f,   0.5f, 0.5f,   m_colour.x, m_colour.y, m_colour.z, m_colour.w,    m_albedo
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

    this->transform();

    return *this;
}