#include "Scene.hpp"

void Scene::pushShape(Shape shape)
{
    shapes.push_back(shape);
    this->compile();
}

void Scene::pushCube(glm::vec3 position, glm::vec4 colour, float scale, float albedo)
{
    Cube cube(position, colour, scale, albedo);
    shapes.push_back(cube);
    this->compile();
}

void Scene::pushPyramid(glm::vec3 position, glm::vec4 colour, float scale, float albedo)
{
    Pyramid pyramid(position, colour, scale, albedo);
    shapes.push_back(pyramid);
    this->compile();
}

void Scene::removeShape(int index)
{
    shapes.erase(shapes.begin() + index);
    this->compile();
}

void Scene::compile()
{
    vertices.clear();
    indices.clear();
    normals.clear();

    for(int i = 0; i < shapes.size(); i++)
    {
        shapes[i].push(vertices, indices, normals);
    }
}

void Scene::applyMvp(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    glm::mat4 modelView = view * model;
    glm::mat4 mvp = projection * modelView;
    glm::mat4 normalMvp = projection * transpose(inverse(modelView));

    for(int i = 0; i < vertices.size(); i += 10)
    {
        glm::vec4 modelVertex(vertices[i], vertices[i+1], vertices[i+2], 1.0f);
        glm::vec4 homogenousVertex = mvp * modelVertex;
        vertices[i] = homogenousVertex.x;
        vertices[i+1] = homogenousVertex.y;
        vertices[i+2] = homogenousVertex.z;
    }

    for(int i = 0; i < normals.size(); i += 3)
    {
        glm::vec4 modelNormal(normals[i], normals[i+1], normals[i+2], 1.0f);
        glm::vec4 homogenousNormal = normalMvp * modelNormal;
        normals[i] = homogenousNormal.x;
        normals[i+1] = homogenousNormal.y;
        normals[i+2] = homogenousNormal.z;
    }
}