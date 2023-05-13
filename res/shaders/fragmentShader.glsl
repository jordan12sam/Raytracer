#version 460 core

in vec2 UVs;
out vec4 FragColor;

uniform float vertices[1024];
uniform int indices[1024];
uniform int numVertices;
uniform int vertexSize;
uniform int numIndices;
uniform mat4 mvp;
uniform float AR;

//Möller–Trumbore intersection algorithm
bool intersectRayTriangle(vec3 rayDirection, vec3 v0, vec3 v1, vec3 v2, out vec3 intersection)
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
    vec3 s = -v0;
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
        intersection = rayDirection * t;
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
    // Scale y coordinates to [-1.0, 1.0]
    // And x coordinates to [-AR, AR]
	float x = (2 * UVs.x - 1) * AR;
	float y = 2 * UVs.y - 1;

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

        float fov = 90.0;
        vec3 pixelOrigin = vec3(x, y, 0.5 / tan(radians(fov/2)));
        vec3 rayDirection = normalize(pixelOrigin);
        vec3 intersection;
        bool intersects = intersectRayTriangle(rayDirection, pos0,  pos1, pos2, intersection);

        if (intersects)
        {
            intersectsAny = true;
            if (length(intersection) < length(closestIntersection))
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

	FragColor = pixelColour;
}