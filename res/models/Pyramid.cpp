#include "Shape.hpp"

Shape Shape::setPyramid()
{
    vertices =
    {
        // Base
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     1.0, 0.0, 0.0, 1.0,     // Vertex 0
        -0.5f, -0.5f, 0.5f,     0.0f, 1.0f,     0.0, 1.0, 0.0, 1.0,     // Vertex 1
        0.5f,  -0.5f, 0.5f,     1.0f, 1.0f,     0.0, 0.0, 1.0, 1.0,     // Vertex 2
        0.5f,  -0.5f, -0.5f,    1.0f, 0.0f,     0.0, 0.0, 0.0, 1.0,     // Vertex 3

        // Front face
        0.0f, 0.5f, 0.0f,       0.5f, 0.5f,     1.0, 1.0, 0.0, 1.0,     // Vertex 4
    };

    indices =
    {
        // Base
        0, 1, 2,
        2, 3, 0,

        // Front face
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

    this->translate();

    return *this;
}