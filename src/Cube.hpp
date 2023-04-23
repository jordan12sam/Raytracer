#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Cube {
    public:
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
        int verticesCount = sizeof(vertices)/sizeof(vertices[0]);
        int indicesSize = sizeof(indices);
        int indicesCount = sizeof(indices)/sizeof(indices[0]);

        void spawn( std::vector<float> &sceneVertices, std::vector<int> &sceneIndices, glm::vec3 position, glm::vec3 scale, glm::vec4 colour){

            glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
            glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), position);
            
            for (int i = 0; i < verticesCount; i += 5) {

                glm::vec4 coordinates(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f);
                glm::vec2 texture(vertices[i + 3], vertices[i + 4]);

                coordinates = translateMat * scaleMat * coordinates;

                sceneVertices.push_back(coordinates.x);
                sceneVertices.push_back(coordinates.y);
                sceneVertices.push_back(coordinates.z);

                sceneVertices.push_back(texture.x);
                sceneVertices.push_back(texture.y);

                sceneVertices.push_back(colour.x);
                sceneVertices.push_back(colour.y);
                sceneVertices.push_back(colour.z);
                sceneVertices.push_back(colour.w);
            }

            int cubeNum = sceneIndices.size() / indicesCount;
            for (int i = 0; i < indicesCount; i++)
            {
                sceneIndices.push_back(indices[i] + cubeNum * 24);
            } 
        }
};