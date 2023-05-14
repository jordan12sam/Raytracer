#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "../res/models/Shape.hpp"

class Scene {
    public:
        std::vector<GLfloat> worldVertices;
        std::vector<GLfloat> worldNormals;

        std::vector<GLfloat> projectionVertices;
        std::vector<GLfloat> projectionNormals;

        std::vector<GLint> indices;

        int vertexSize = 10;

        void transform(glm::mat4 MVP, glm::mat4 normalMVP, std::vector<GLfloat> &vert, std::vector<GLfloat> &norm, std::vector<GLint> &ind);

        Scene withCube(glm::vec3 position, float scale, glm::vec4 colour, float albedo);
        Scene withPyramid(glm::vec3 position, float scale, glm::vec4 colour, float albedo);
    private:
        void transformVertices(glm::mat4 MVP);
        void transformNormals(glm::mat4 normalMVP);

};