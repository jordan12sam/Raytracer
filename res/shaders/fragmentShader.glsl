#version 460 core

in vec2 UVs;
out vec4 FragColor;

uniform sampler2D screen;

void main()
{
	FragColor = texture(screen, UVs);
}