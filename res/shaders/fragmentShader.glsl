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
uniform mat4 normalMVP;
uniform float AR;

const float PI = 3.141592653589793238;

struct HitInfo {
    bool didHit;
    vec3 position;
    vec3 normal;
};

struct Primitive {
    HitInfo hitInfo;
    vec3 norm;
    vec3 pos0, pos1, pos2;
    vec2 tex0, tex1, tex2;
    vec4 col0, col1, col2;
    float alb0, alb1, alb2;
};

struct Ray {
    vec3 origin;
    vec3 direction;
    vec4 colour;
};

float randNormalDist(vec2 co){
    float mean = 0.5;
    float stddev = 0.166667;
    float a = sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453;
    float b = sin(dot(co, vec2(4.1414, 65.982))) * 54736.7893;
    float rand = sqrt(-2.0 * log(a)) * cos(2.0 * PI * b);
    return mean + stddev * rand;
}

vec3 randomDirection(vec2 co, Primitive triangle){
    float azimuth = randNormalDist(co) * 2.0 * PI;
    float inclination = randNormalDist(co + vec2(1.0, 0.0)) * 2.0 - 1.0;
    float cosInclination = sqrt(1.0 - inclination * inclination);
    float x = cos(azimuth) * cosInclination;
    float y = sin(azimuth) * cosInclination;
    float z = inclination;
    vec3 direction = normalize(vec3(x, y, z));
    if (dot(direction, triangle.norm) < 0.0) {
        direction = reflect(direction, direction);
    }
    return normalize(direction);
}


//Möller–Trumbore intersection algorithm
void intersectRayTriangle(Ray ray, out Primitive triangle)
{
    const float EPSILON = 0.000001;

    vec3 edge1 = triangle.pos1 - triangle.pos0;
    vec3 edge2 = triangle.pos2 - triangle.pos0;
    vec3 h = cross(ray.direction, edge2);
    float a = dot(edge1, h);
    if (a > -EPSILON && a < EPSILON) {
        triangle.hitInfo.didHit = false; // ray is parallel to triangle
        return;
    }

    float f = 1.0 / a;
    vec3 s = -triangle.pos0;
    float u = f * dot(s, h);
    if (u < 0.0 || u > 1.0) {
        triangle.hitInfo.didHit = false; // intersection is outside the triangle
        return;
    }

    vec3 q = cross(s, edge1);
    float v = f * dot(ray.direction, q);
    if (v < 0.0 || u + v > 1.0) {
        triangle.hitInfo.didHit = false; // intersection is outside the triangle
        return;
    }

    float t = f * dot(edge2, q);
    if (t > EPSILON) {
        triangle.hitInfo.position = ray.direction * t;
        triangle.hitInfo.didHit = true; // intersection is valid
        return;
    }

    triangle.hitInfo.didHit = false; // intersection is behind the ray
}

vec3 barycentric(Primitive triangle) {
    vec3 v0 = triangle.pos1 - triangle.pos0;
    vec3 v1 = triangle.pos2 - triangle.pos0;
    vec3 v2 = triangle.hitInfo.position - triangle.pos0;

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

vec4 interpolateColour(vec3 barycentricCoords, Primitive triangle) {
    return triangle.col0 * barycentricCoords.x + triangle.col1 * barycentricCoords.y + triangle.col2 * barycentricCoords.z;
}

vec2 interpolateTexture(vec3 barycentricCoords, Primitive triangle) {
    return triangle.tex0 * barycentricCoords.x + triangle.tex1 * barycentricCoords.y + triangle.tex2 * barycentricCoords.z;
}

float interpolateAlbedo(vec3 barycentricCoords, Primitive triangle) {
    return triangle.alb0 * barycentricCoords.x + triangle.alb1 * barycentricCoords.y + triangle.alb2 * barycentricCoords.z;
}

void getPrimitive(  int i,
                    out Primitive triangle)
{
    triangle.norm = vec3(normals[i], normals[i + 1], normals[i + 2]);

    triangle.pos0 = vec3(vertices[indices[i]     * vertexSize    ],
                        vertices[indices[i]     * vertexSize + 1],
                        vertices[indices[i]     * vertexSize + 2]);
    triangle.pos1 = vec3(vertices[indices[i + 1] * vertexSize    ],
                        vertices[indices[i + 1] * vertexSize + 1],
                        vertices[indices[i + 1] * vertexSize + 2]);
    triangle.pos2 = vec3(vertices[indices[i + 2] * vertexSize    ],
                        vertices[indices[i + 2] * vertexSize + 1],
                        vertices[indices[i + 2] * vertexSize + 2]);

    triangle.tex0 = vec2(vertices[indices[i]     * vertexSize + 3],
                        vertices[indices[i]     * vertexSize + 4]);
    triangle.tex1 = vec2(vertices[indices[i + 1] * vertexSize + 3],
                        vertices[indices[i + 1] * vertexSize + 4]);
    triangle.tex2 = vec2(vertices[indices[i + 2] * vertexSize + 3],
                        vertices[indices[i + 2] * vertexSize + 4]);

    triangle.col0 = vec4(vertices[indices[i]     * vertexSize + 5],
                        vertices[indices[i]     * vertexSize + 6],
                        vertices[indices[i]     * vertexSize + 7],
                        vertices[indices[i]     * vertexSize + 8]);
    triangle.col1 = vec4(vertices[indices[i + 1] * vertexSize + 5],
                        vertices[indices[i + 1] * vertexSize + 6],
                        vertices[indices[i + 1] * vertexSize + 7],
                        vertices[indices[i + 1] * vertexSize + 8]);
    triangle.col2 = vec4(vertices[indices[i + 2] * vertexSize + 5],
                        vertices[indices[i + 2] * vertexSize + 6],
                        vertices[indices[i + 2] * vertexSize + 7],
                        vertices[indices[i + 2] * vertexSize + 8]);
    
    triangle.alb0 =      vertices[indices[i]     * vertexSize + 9];
    triangle.alb1 =      vertices[indices[i + 1] * vertexSize + 9];
    triangle.alb2 =      vertices[indices[i + 2] * vertexSize + 9];
}

void reflection(out Ray ray)
{
    //Intersection information
    vec3 closestIntersection = vec3(10000.0);
    bool intersectsAny = false;
    vec4 colour = vec4(0.0, 0.2, 0.2, 1.0);;
    float albedo;
    vec3 normal;

    //Primitive information
    Primitive triangle;

    //For each primitive
	for(int i = 0; i < numIndices; i += 3)
	{
        //Get primitive info
        getPrimitive(i, triangle);


        //Transform primitive into model view
        triangle.pos0 = (MVP * vec4(triangle.pos0, 1.0)).xyz;
        triangle.pos1 = (MVP * vec4(triangle.pos1, 1.0)).xyz;
        triangle.pos2 = (MVP * vec4(triangle.pos2, 1.0)).xyz;
        triangle.norm = (normalMVP * vec4(triangle.norm, 1.0)).xyz;

        //Checks for intersection
        intersectRayTriangle(ray, triangle);

        //If this is the closest intersection, then update values
        if (triangle.hitInfo.didHit && length(triangle.hitInfo.position) < length(closestIntersection))
        {
            intersectsAny = true;
            closestIntersection = triangle.hitInfo.position;

            vec3 barycentricCoords = barycentric(triangle);
            //colour = interpolateColour(barycentricCoords, col0, col1, col2);
            albedo = interpolateAlbedo(barycentricCoords, triangle);
            normal = triangle.norm;
            colour = vec4(normal * 0.5 + 0.5, 1.0);
        }
	}

    //Return ray information
    ray.direction = reflect(ray.direction, normal);
    ray.origin = closestIntersection;
    ray.colour = colour;
}

void main()
{
    // Scale y coordinates to [-1.0, 1.0]
    // And x coordinates to [-AR, AR]
	float x = (2 * screen.x - 1);
	float y = (2 * screen.y - 1);

    // Initialise ray properties
    Ray ray;
    float fov = 90.0;
    ray.origin = vec3(0.0, 0.0, 0.0);
    vec3 pixelOrigin = vec3(x, y, 0.5 / tan(radians(fov/2)));
    ray.direction = normalize(pixelOrigin);
    ray.colour;

    // Calculate pixel colour
    reflection(ray);

    // Draw colour
	FragColor = ray.colour;
}