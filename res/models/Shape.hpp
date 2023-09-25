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
        Shape::Shape(glm::vec3 position, glm::vec4 colour, float scale, float albedo, float textured)
        : m_position(position), m_colour(colour), m_scale(scale), m_albedo(albedo), m_textured(textured) {}
        ~Shape();
        void push(std::vector<GLfloat> &sceneVertices, std::vector<GLint> &sceneIndices, std::vector<GLfloat> &sceneNormals);
    protected:
        float m_scale;
        float m_albedo;
        float m_textured;
        glm::vec3 m_position;
        glm::vec4 m_colour;
        std::vector<GLfloat> m_vertices;
        std::vector<GLint> m_indices;
        std::vector<GLfloat> m_normals;
        void transform(); 
};

class Cube: public Shape
{
    public:
        Cube(glm::vec3 position, glm::vec4 colour, float scale, float albedo, float textured);
};

class Pyramid: public Shape
{
    public:
        Pyramid(glm::vec3 position, glm::vec4 colour, float scale, float albedo, float textured);
};
