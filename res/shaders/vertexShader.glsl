#version 460 core

uniform mat4 inverseProjectionView;
uniform float near;
uniform float far;

layout (location = 0) in vec2 pos;

out vec2 screen;
out vec3 originn;
out vec3 rayy;

void main()
{
    gl_Position = vec4(pos, 0.0, 1.0);
    screen = pos;

    originn = vec3(pos, -1.0) * near;
    rayy = vec3(pos * (far - near), far + near);
}