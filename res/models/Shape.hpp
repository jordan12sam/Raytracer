#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

class Shape
{
    public:
        Shape(glm::vec3 position): m_position(position){};
        ~Shape();
        void push(std::vector<GLfloat> &sceneVertices, std::vector<GLint> &sceneIndices);
        Shape setCube();
    protected:
        glm::vec3 m_position;
        std::vector<GLfloat> vertices;
        std::vector<GLint> indices;
        void translate(); 
};
