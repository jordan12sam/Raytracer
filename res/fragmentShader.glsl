#version 430 core

out vec4 FragColour;
in vec2 UVs;
in vec4 Colour;

uniform sampler2D screen;

void main()
{
   FragColour = texture(screen, UVs);
}