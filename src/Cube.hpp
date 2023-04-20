struct Cube
{
    GLfloat vertices[72] = {
        // front face
        -0.5f, -0.5f,  0.5f, // bottom left
        0.5f, -0.5f,  0.5f, // bottom right
        0.5f,  0.5f,  0.5f, // top right
        -0.5f,  0.5f,  0.5f, // top left

        // back face
        -0.5f, -0.5f, -0.5f, // bottom left
        0.5f, -0.5f, -0.5f, // bottom right
        0.5f,  0.5f, -0.5f, // top right
        -0.5f,  0.5f, -0.5f, // top left

        // right face
        0.5f, -0.5f,  0.5f, // bottom front
        0.5f, -0.5f, -0.5f, // bottom back
        0.5f,  0.5f, -0.5f, // top back
        0.5f,  0.5f,  0.5f, // top front

        // left face
        -0.5f, -0.5f,  0.5f, // bottom front
        -0.5f, -0.5f, -0.5f, // bottom back
        -0.5f,  0.5f, -0.5f, // top back
        -0.5f,  0.5f,  0.5f, // top front

        // top face
        -0.5f,  0.5f,  0.5f, // front left
        0.5f,  0.5f,  0.5f, // front right
        0.5f,  0.5f, -0.5f, // back right
        -0.5f,  0.5f, -0.5f, // back left

        // bottom face
        -0.5f, -0.5f,  0.5f, // front left
        0.5f, -0.5f,  0.5f, // front right
        0.5f, -0.5f, -0.5f, // back right
        -0.5f, -0.5f, -0.5f  // back left
    };

    GLuint indices[36] = {
        // front face
        0, 1, 2,
        2, 3, 0,

        // back face
        4, 5, 6,
        6, 7, 4,

        // right face
        8, 9, 10,
        10, 11, 8,

        // left face
        12, 13, 14,
        14, 15, 12,

        // top face
        16, 17, 18,
        18, 19, 16,

        // bottom face
        20, 21, 22,
        22, 23, 20
    };

    int verticesSize = 72 * sizeof(float);
    int indicesCount = 36;
};