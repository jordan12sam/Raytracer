#version 330 core

layout (location = 0) in vec4 Pos;
layout (location = 1) in vec2 Tex;

uniform mat4 uMVP;

out vec2 vTexCoords;

void main()
{
   gl_Position = uMVP * Pos;
   vTexCoords = Tex;
}