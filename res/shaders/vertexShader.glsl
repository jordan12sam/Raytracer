#version 460 core

uniform mat4 inverseProjectionView;
uniform vec3 cameraPosition;
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

    vec4 clipSpacePos = vec4(pos, near, 1.0);
    vec4 viewSpacePos = inverseProjectionView * clipSpacePos;
    vec3 viewSpacePosition = viewSpacePos.xyz / viewSpacePos.w;
    originn = cameraPosition;
    rayy = viewSpacePosition - originn;
}