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

// Simple hash function
uint hash(uint x) {
    x = ((x >> 16u) ^ x) * 0x45d9f3b;
    x = ((x >> 16u) ^ x) * 0x45d9f3b;
    x = (x >> 16u) ^ x;
    return x;
}

vec3 randomDirection(uint seed) {
    vec3 randomVec;

    seed = hash(seed + 0);
    randomVec.x = float(seed) / float(0xffffffffu);

    seed = hash(seed + 1);
    randomVec.y = float(seed) / float(0xffffffffu);

    seed = hash(seed + 2);
    randomVec.z = float(seed) / float(0xffffffffu);

    return normalize(randomVec);
}

void main()
{
    // Use a changing seed value, such as the frame counter
    uint seed = uint(gl_FragCoord.x) + uint(gl_FragCoord.y) * 1024u;
    vec3 dir = randomDirection(seed);
    FragColor = vec4(dir, 1.0);
}