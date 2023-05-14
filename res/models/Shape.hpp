#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/normal.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

class Shape
{
    public:
        Shape(glm::vec3 position, float scale, glm::vec4 colour, float albedo)
        : m_position(position), m_scale(scale), m_colour(colour), m_albedo(albedo){};
        ~Shape();
        void push(std::vector<GLfloat> &sceneVertices, std::vector<GLint> &sceneIndices, std::vector<GLfloat> &sceneNormals);
        Shape setCube();
        Shape setPyramid();
        int vertexSize = 10;
    protected:
        glm::vec3 m_position;
        float m_scale;
        glm::vec4 m_colour;
        float m_albedo;
        std::vector<GLfloat> m_vertices;
        std::vector<GLint> m_indices;
        std::vector<GLfloat> m_normals;
        void transform(); 
};
