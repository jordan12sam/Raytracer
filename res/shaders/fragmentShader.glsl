#version 460 core

in vec2 screen;
out vec4 FragColor;

uniform float vertices[1024];
uniform int indices[1024];
uniform float normals[1024];
uniform int numVertices;
uniform int vertexSize;
uniform int numIndices;
uniform mat4 MVP;
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

vec3 barycentric(vec3 p, vec3 a, vec3 b, vec3 c) {
    vec3 v0 = b - a;
    vec3 v1 = c - a;
    vec3 v2 = p - a;

    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);

    float denom = d00 * d11 - d01 * d01;

    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0 - v - w;

    return vec3(u, v, w);
}

vec4 interpolateColour(vec3 barycentricCoords, vec4 colourA, vec4 colourB, vec4 colourC) {
    return colourA * barycentricCoords.x + colourB * barycentricCoords.y + colourC * barycentricCoords.z;
}

vec2 interpolateTexture(vec3 barycentricCoords, vec2 textureA, vec2 textureB, vec2 textureC) {
    return textureA * barycentricCoords.x + textureB * barycentricCoords.y + textureC * barycentricCoords.z;
}

float interpolateAlbedo(vec3 barycentricCoords, float albedoA, float albedoB, float albedoC) {
    return albedoA * barycentricCoords.x + albedoB * barycentricCoords.y + albedoC * barycentricCoords.z;
}

void getPrimitive(  int i,
                    out vec3 norm,
                    out vec3 pos0, out vec3 pos1, out vec3 pos2,
                    out vec2 tex0, out vec2 tex1, out vec2 tex2,
                    out vec4 col0, out vec4 col1, out vec4 col2,
                    out float alb0, out float alb1, out float alb2)
{
    norm = vec3(normals[i], normals[i + 1], normals[i + 2]);

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
    
    alb0 =      vertices[indices[i]     * vertexSize + 9];
    alb1 =      vertices[indices[i + 1] * vertexSize + 9];
    alb2 =      vertices[indices[i + 2] * vertexSize + 9];
}

void main()
{
    // Scale y coordinates to [-1.0, 1.0]
    // And x coordinates to [-AR, AR]
	float x = (2 * screen.x - 1);
	float y = (2 * screen.y - 1);

    // Test all primitives for intersection
	// If true, interpolate vertex values
	vec4 pixelColour;
    vec3 closestIntersection = vec3(1000.0);
    bool intersectsAny = false;
	for(int i = 0; i < numIndices; i += 3)
	{
        vec3 norm;
		vec3 pos0, pos1, pos2;
        vec2 tex0, tex1, tex2;
        vec4 col0, col1, col2;
        float alb0, alb1, alb2;

        getPrimitive(i, norm, pos0, pos1, pos2, tex0, tex1, tex2, col0, col1, col2, alb0, alb1, alb2);

        pos0 = (MVP * vec4(pos0, 1.0)).xyz;
        pos1 = (MVP * vec4(pos1, 1.0)).xyz;
        pos2 = (MVP * vec4(pos2, 1.0)).xyz;

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
				vec3 barycentricCoords = barycentric(intersection, pos0, pos1, pos2);
                vec4 col = interpolateColour(barycentricCoords, col0, col1, col2);
				vec2 tex = interpolateTexture(barycentricCoords, tex0, tex1, tex2);
                float alb = interpolateAlbedo(barycentricCoords, alb0, alb1, alb2);
				if(barycentricCoords.x < 0.01 || barycentricCoords.y < 0.01 || barycentricCoords.z < 0.01 )
				{
					pixelColour = vec4(0.0, 0.0, 0.0, 1.0);
				}
				else
				{
					pixelColour = vec4(norm * 0.5 + 0.5, 1.0);
				}

            }
        }
	}

    // Write background colour
    if (!intersectsAny)
    {
        pixelColour = vec4(0.0, 0.2, 0.2, 1.0);
    }

	FragColor = pixelColour;
}