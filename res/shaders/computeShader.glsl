#version 460 core

layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D screen;

void main()
{
    // Scale pixel coordinates to [0.0, 1.0]
	ivec2 pixelCoordinates = ivec2(gl_GlobalInvocationID.xy);
	ivec2 dimensions = imageSize(screen);
	float x = (float(pixelCoordinates.x) / dimensions.x);
	float y = (float(pixelCoordinates.y) / dimensions.y);

    vec4 pixelColour = vec4(x, y, 0.0, 1.0);

	imageStore(screen, pixelCoordinates, pixelColour);
}