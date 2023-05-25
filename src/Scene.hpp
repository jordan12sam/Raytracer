#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../res/models/Shape.hpp"

#include <iostream>
#include <string>

class Scene {
    public:
        void pushShape(Shape shape);
        void pushCube(glm::vec3 position, glm::vec4 colour, float scale, float albedo);
        void pushPyramid(glm::vec3 position, glm::vec4 colour, float scale, float albedo);
        void removeShape(int index);
        void compile();
        std::vector<Shape> shapes;
        std::vector<GLfloat> vertices;
        std::vector<GLint> indices;
        std::vector<GLfloat> normals;

};