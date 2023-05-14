#include "Scene.hpp"

Scene Scene::withCube(glm::vec3 position, float scale, glm::vec4 colour, float albedo)
{
    Shape cube(position, scale, colour, albedo);
    cube.setCube().push(worldVertices, indices, worldNormals);
    return *this;
}

Scene Scene::withPyramid(glm::vec3 position, float scale, glm::vec4 colour, float albedo)
{
    Shape pyramid(position, scale, colour, albedo);
    pyramid.setCube().push(worldVertices, indices, worldNormals);
    return *this;
}

void Scene::transform(glm::mat4 MVP, glm::mat4 normalMVP, std::vector<GLfloat> &vert, std::vector<GLfloat> &norm, std::vector<GLint> &ind)
{
    this->transformVertices(MVP);
    this->transformNormals(normalMVP);
    
    vert = projectionVertices;
    norm = projectionNormals;
    ind = indices;
};

void Scene::transformVertices(glm::mat4 MVP)
{
    for (int i = 0; i < worldVertices.size(); i += vertexSize)
    {
        glm::vec3 vertex(worldVertices[i], worldVertices[i + 1], worldVertices[i + 2]);
        vertex = glm::vec3(MVP * glm::vec4(vertex, 1.0f));
        projectionVertices.push_back(vertex.x);
        projectionVertices.push_back(vertex.y);
        projectionVertices.push_back(vertex.z);
        for (int j = 3; j < vertexSize; j++)
        {
            projectionVertices.push_back(worldVertices[j]);
        }
    }
};

void Scene::transformNormals(glm::mat4 normalMVP)
{
    for (int i = 0; i < worldNormals.size(); i += 3)
    {
        glm::vec3 vertex(worldNormals[i], worldNormals[i + 1], worldNormals[i + 2]);
        vertex = glm::vec3(normalMVP * glm::vec4(vertex, 1.0f));
        projectionNormals.push_back(vertex.x);
        projectionNormals.push_back(vertex.y);
        projectionNormals.push_back(vertex.z);
    }   
};