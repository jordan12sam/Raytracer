struct Cube
{
    GLfloat vertices[120] = {
                            //Positions             //Texture
        // front face
        /* bottom left */   -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
        /* bottom right */  0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 
        /* top right */     0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
        /* top left */      -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,

        // back face
        /* bottom left */   -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
        /* bottom right */  0.5f, -0.5f, -0.5f,     1.0f, 0.0f,
        /* top right */     0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
        /* top left */      -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,

        // right face
        /*bottom front*/    0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
        /*bottom back*/     0.5f, -0.5f, -0.5f,     1.0f, 0.0f,
        /*top back*/        0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
        /*top front*/       0.5f,  0.5f,  0.5f,     0.0f, 1.0f,

        // left face
        /*bottom front*/    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
        /*bottom back*/     -0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
        /*top back*/        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
        /*top front*/       -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,

        // top face
        /*front left*/      -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,
        /*front right*/     0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
        /*back right*/      0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
        /*back left*/       -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,

        // bottom face
        /*front left*/      -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
        /*front right*/     0.5f, -0.5f,  0.5f,     1.0f, 0.0f,
        /*back right*/      0.5f, -0.5f, -0.5f,     1.0f, 1.0f,
        /*back left*/       -0.5f, -0.5f, -0.5f,    0.0f, 1.0f
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

    int verticesSize = sizeof(vertices);
    int indicesCount = sizeof(indices)/sizeof(GLuint);
};