#include "Shape.hpp"

Cube::Cube(glm::vec3 position, glm::vec4 colour, float scale, float albedo)
: Shape(position, colour, scale, albedo)
{
    m_vertices =
    {
        //Position              //Texture       //Colour                                        //Albedo
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,
        -0.5f, 0.5f, -0.5f,     0.0f, 1.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,    
        0.5f, 0.5f, -0.5f,      1.0f, 1.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,     
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,   

        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,       
        -0.5f,  0.5f, 0.5f,     0.0f, 1.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,     
        0.5f,  0.5f, 0.5f,      1.0f, 1.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,   
        0.5f, -0.5f, 0.5f,      1.0f, 0.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,       

        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,     
        -0.5f,  0.5f, 0.5f,     0.0f, 1.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,    
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,      
        -0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,   

        0.5f, -0.5f, -0.5f,     0.0f, 0.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,      
        0.5f,  0.5f, -0.5f,     0.0f, 1.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,        
        0.5f,  0.5f, 0.5f,      1.0f, 1.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,     
        0.5f, -0.5f, 0.5f,      1.0f, 0.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,      

        -0.5f, 0.5f, -0.5f,     0.0f, 0.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,     
        -0.5f, 0.5f, 0.5f,      0.0f, 1.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,      
        0.5f,   0.5f, 0.5f,     1.0f, 1.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,     
        0.5f,   0.5f, -0.5f,    1.0f, 0.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,   

        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,       
        -0.5f, -0.5f, 0.5f,     0.0f, 1.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,       
        0.5f,  -0.5f, 0.5f,     1.0f, 1.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo,     
        0.5f,  -0.5f, -0.5f,    1.0f, 0.0f,     m_colour.x,m_colour.y, m_colour.z, m_colour.w,  m_albedo  
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
