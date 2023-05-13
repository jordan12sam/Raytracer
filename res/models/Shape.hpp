#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

class Shape
{
    public:
        Shape(glm::vec3 position, float scale): m_position(position), m_scale(scale){};
        ~Shape();
        void push(std::vector<GLfloat> &sceneVertices, std::vector<GLint> &sceneIndices);
        Shape setCube();
        Shape setPyramid();
    protected:
        glm::vec3 m_position;
        float m_scale;
        std::vector<GLfloat> vertices;
        std::vector<GLint> indices;
        void transform(); 
};
