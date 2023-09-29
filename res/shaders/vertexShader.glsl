#version 460 core

uniform float AR;

layout (location = 0) in vec2 pos;

out vec2 screen;

void main()
{
    gl_Position = vec4(pos, 0.0, 1.0);
    screen = vec2(pos.x * AR, pos.y);
}