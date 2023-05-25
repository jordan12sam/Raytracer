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