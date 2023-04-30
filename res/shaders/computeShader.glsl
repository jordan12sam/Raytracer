#version 460 core

layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D screen;

uniform float vertices[1024];
uniform int indices[1024];
uniform int numVertices;
uniform int vertexSize;
uniform int numIndices;
uniform mat4 mvp;

bool intersectRayTriangle(vec3 rayOrigin, vec3 rayDirection, vec3 v0, vec3 v1, vec3 v2, out vec3 intersection)
{
    const float EPSILON = 0.000001;

    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0;
    vec3 h = cross(rayDirection, edge2);
    float a = dot(edge1, h);
    if (a > -EPSILON && a < EPSILON) {
        return false; // ray is parallel to triangle
    }

    float f = 1.0 / a;
    vec3 s = rayOrigin - v0;
    float u = f * dot(s, h);
    if (u < 0.0 || u > 1.0) {
        return false; // intersection is outside the triangle
    }

    vec3 q = cross(s, edge1);
    float v = f * dot(rayDirection, q);
    if (v < 0.0 || u + v > 1.0) {
        return false; // intersection is outside the triangle
    }

    float t = f * dot(edge2, q);
    if (t > EPSILON) {
        intersection = rayOrigin + rayDirection * t;
        return true; // intersection is valid
    }

    return false; // intersection is behind the ray
}

void getPrimitive(  int i,
                    out vec3 pos0, out vec3 pos1, out vec3 pos2,
                    out vec2 tex0, out vec2 tex1, out vec2 tex2,
                    out vec4 col0, out vec4 col1, out vec4 col2)
{
    pos0 = vec3(vertices[indices[i]     * vertexSize    ],
                vertices[indices[i]     * vertexSize + 1],
                vertices[indices[i]     * vertexSize + 2]);
    pos1 = vec3(vertices[indices[i + 1] * vertexSize    ],
                vertices[indices[i + 1] * vertexSize + 1],
                vertices[indices[i + 1] * vertexSize + 2]);
    pos2 = vec3(vertices[indices[i + 2] * vertexSize    ],
                vertices[indices[i + 2] * vertexSize + 1],
                vertices[indices[i + 2] * vertexSize + 2]);

    tex0 = vec2(vertices[indices[i]     * vertexSize + 3],
                vertices[indices[i]     * vertexSize + 4]);
    tex1 = vec2(vertices[indices[i + 1] * vertexSize + 3],
                vertices[indices[i + 1] * vertexSize + 4]);
    tex2 = vec2(vertices[indices[i + 2] * vertexSize + 3],
                vertices[indices[i + 2] * vertexSize + 4]);

    col0 = vec4(vertices[indices[i]     * vertexSize + 5],
                vertices[indices[i]     * vertexSize + 6],
                vertices[indices[i]     * vertexSize + 7],
                vertices[indices[i]     * vertexSize + 8]);
    col1 = vec4(vertices[indices[i + 1] * vertexSize + 5],
                vertices[indices[i + 1] * vertexSize + 6],
                vertices[indices[i + 1] * vertexSize + 7],
                vertices[indices[i + 1] * vertexSize + 8]);
    col2 = vec4(vertices[indices[i + 2] * vertexSize + 5],
                vertices[indices[i + 2] * vertexSize + 6],
                vertices[indices[i + 2] * vertexSize + 7],
                vertices[indices[i + 2] * vertexSize + 8]);
}

void main()
{
    // Scale pixel coordinates to [0.0, 1.0]
	ivec2 pixelCoordinates = ivec2(gl_GlobalInvocationID.xy);
	ivec2 dimensions = imageSize(screen);
	float x = (float(pixelCoordinates.x) / dimensions.x);
	float y = (float(pixelCoordinates.y) / dimensions.y);

    // Test all primitives for intersection
    vec3 closestIntersection;
    bool intersectsAny = false;
	for(int i = 0; i < numIndices; i += 3)
	{
        vec3 pos0, pos1, pos2;
        vec2 tex0, tex1, tex2;
        vec4 col0, col1, col2;
        getPrimitive(i, pos0, pos1, pos2, tex0, tex1, tex2, col0, col1, col2);

        pos0 = (vec4(pos0, 1.0) * mvp).xyz;
        pos1 = (vec4(pos1, 1.0) * mvp).xyz;
        pos2 = (vec4(pos2, 1.0) * mvp).xyz;

        vec3 cameraOrigin = vec3(0.0, 0.0, -1.0);
        vec3 pixelOrigin = vec3(x, y, 0.0);
        vec3 rayDirection = normalize(pixelOrigin - cameraOrigin);
        vec3 intersection;
        bool intersects = intersectRayTriangle(cameraOrigin, rayDirection, pos0,  pos1, pos2, intersection);

        if (intersects)
        {
            intersectsAny = true;

            vec3 edge1 = intersection - cameraOrigin;
            vec3 edge2 = closestIntersection - cameraOrigin;

            if (length(edge1) < length(edge2))
            {
                closestIntersection = intersection;
            }
        }
	}

    // Write colour
    vec4 pixelColour;
    if (intersectsAny)
    {
        pixelColour = vec4(0.0, 1.0, 0.0, 1.0);
    }
    else
    {
        pixelColour = vec4(1.0, 0.0, 0.0, 1.0);
    }

	imageStore(screen, pixelCoordinates, pixelColour);
}