#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../res/models/Shape.hpp"

#include <iostream>
#include <string>

class Scene {
    public:
        Scene() {};
        Scene(Scene& scene);
        void pushShape(Shape shape);
        void pushCube(glm::vec3 position, glm::vec4 colour, float scale, float albedo, float textured);
        void pushPyramid(glm::vec3 position, glm::vec4 colour, float scale, float albedo, float textured);
        void removeShape(int index);
        void compile();
        void applyMvp(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
        std::vector<Shape> shapes;
        std::vector<GLfloat> vertices;
        std::vector<GLint> indices;
        std::vector<GLfloat> normals;

};