#include "Scene.hpp"

Scene::Scene(Scene& scene)
{
    for(int i = 0; i < scene.shapes.size(); i++)
    {
        shapes.push_back(scene.shapes[i]);
    }
    for(int i = 0; i < scene.indices.size(); i++)
    {
        indices.push_back(scene.indices[i]);
    }
    worldSpace = scene.worldSpace;
    cameraSpace = scene.cameraSpace;
}

void Scene::pushShape(Shape shape)
{
    shapes.push_back(shape);
    this->compile();
}

void Scene::pushCube(glm::vec4 position, glm::vec4 colour, float scale, float albedo, float textured)
{
    Cube cube(position, colour, scale, albedo, textured);
    shapes.push_back(cube);
    this->compile();
}

void Scene::pushPyramid(glm::vec4 position, glm::vec4 colour, float scale, float albedo, float textured)
{
    Pyramid pyramid(position, colour, scale, albedo, textured);
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
    indices.clear();
    worldSpace.vertices.clear();
    worldSpace.normals.clear();

    for(int i = 0; i < shapes.size(); i++)
    {
        shapes[i].push(worldSpace.vertices, indices, worldSpace.normals);
    }
}

void Scene::applyMvp(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    cameraSpace.vertices.clear();
    cameraSpace.normals.clear();

    glm::mat4 modelView = view * model;
    glm::mat4 mvp = projection * modelView;
    glm::mat4 normalMvp = projection * transpose(inverse(modelView)); 

    for(int i = 0; i < worldSpace.vertices.size(); i++)
    {
        cameraSpace.vertices.push_back(worldSpace.vertices[i]);
        cameraSpace.vertices[i].position = mvp * worldSpace.vertices[i].position;
    }

    cameraSpace.light = mvp * worldSpace.light;

    for(int i = 0; i < worldSpace.normals.size(); i += 3)
    {
        glm::vec4 modelNormal(worldSpace.normals[i], worldSpace.normals[i+1], worldSpace.normals[i+2], 1.0f);
        glm::vec4 homogenousNormal = normalMvp * modelNormal;
        cameraSpace.normals.push_back(homogenousNormal.x);
        cameraSpace.normals.push_back(homogenousNormal.y);
        cameraSpace.normals.push_back(homogenousNormal.z);
    }
}