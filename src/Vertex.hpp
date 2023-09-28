#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#pragma once

struct Vertex {
    float textured;
    float albedo;
    glm::vec2 texture;
    glm::vec4 position;
    glm::vec4 colour;

    Vertex(float textured, float albedo, glm::vec2 texture, glm::vec4 position, glm::vec4 colour)
        : textured(textured), albedo(albedo), texture(texture), position(position), colour(colour) {}

    Vertex() = default;
};