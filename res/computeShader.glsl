#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) readonly buffer VertexBuffer {
    vec4 vertices[];
};

layout(std430, binding = 1) readonly buffer IndexBuffer {
    uint indices[];
};

layout(rgba32f, binding = 2) uniform image2D Screen;

uniform vec3 cameraPos;
uniform ivec2 screenSize;

void main() {
    vec4 pixel = vec4(1.0, 0.0, 0.0, 1.0);
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);
    imageStore(Screen, pixelCoord, pixel);
}