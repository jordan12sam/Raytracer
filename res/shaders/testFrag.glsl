#version 460 core

in vec2 screen;
in vec3 originn;
in vec3 rayy;
out vec4 FragColor;

uniform float vertices[1200];
uniform int indices[1200];
uniform float normals[1200];
uniform int numVertices;
uniform int vertexSize;
uniform int numIndices;
uniform float AR;

uniform sampler2D textureSampler;

struct Vertex {
    float textured;
    float albedo;
    vec2 texture;
    vec4 position;
    vec4 colour;
};

layout(std140, binding = 0) buffer VertexBuffer {
    Vertex verticesSSBO[];
};

void main()
{
    if (bool(verticesSSBO[0].textured)){
        FragColor = vec4(1.0);
    }
    else{
        FragColor = vec4(0.0);
    }
}