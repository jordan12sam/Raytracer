#version 330 core

layout (location = 0) in vec4 Pos;
layout (location = 1) in vec2 Tex;
layout (location = 2) in vec4 Col;

uniform mat4 uMVP;

out vec2 vTex;
out vec4 vCol;

void main()
{
   gl_Position = uMVP * Pos;
   vTex = Tex;
   vCol = Col;
}