#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec2 screen;

void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
	screen = tex;
}