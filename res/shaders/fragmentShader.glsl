#version 460 core

#pragma optimize(off)

#define MAX_BOUNCES 5

in vec2 screen;
out vec4 FragColor;

uniform int indices[1200];
uniform int numVertices;
uniform int vertexSize;
uniform int numIndices;
uniform sampler2D textureSampler;

const float PI = 3.141592653589793238;

struct Vertex {
    float textured;
    float albedo;
    vec2 texture;
    vec4 position;
    vec4 colour;
};

layout(std140, binding = 0) buffer VertexBuffer {
    Vertex vertices[];
};

struct HitInfo {
    bool didHit;
    vec3 position;
    vec3 normal;
};

struct Primitive {
    HitInfo hitInfo;
    Vertex vertices[3];
};

struct Ray {
    vec3 origin;
    vec3 direction;
    vec4 colour;
};

uint hash(uint x) {
    x = ((x >> 16u) ^ x) * 0x45d9f3b;
    x = ((x >> 16u) ^ x) * 0x45d9f3b;
    x = (x >> 16u) ^ x;
    return x;
}

vec3 randomDirection(uint seed) {
    vec3 randomVec;

    seed = hash(seed + 0);
    randomVec.x = float(seed) / float(0xffffffffu);

    seed = hash(seed + 1);
    randomVec.y = float(seed) / float(0xffffffffu);

    seed = hash(seed + 2);
    randomVec.z = float(seed) / float(0xffffffffu);

    return normalize(randomVec);
}


//Möller–Trumbore intersection algorithm
void intersectRayTriangle(Ray ray, out Primitive triangle)
{
    const float EPSILON = 0.000000000000001;

    vec3 edge1 = vec3(triangle.vertices[1].position - triangle.vertices[0].position);
    vec3 edge2 = vec3(triangle.vertices[2].position - triangle.vertices[0].position);
    vec3 h = cross(ray.direction, edge2);
    float a = dot(edge1, h);
    if (a > -EPSILON && a < EPSILON) {
        triangle.hitInfo.didHit = false; // ray is parallel to triangle
        return;
    }

    float f = 1.0 / a;
    vec3 s = -vec3(triangle.vertices[0].position);
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
    vec3 v0 = vec3(triangle.vertices[1].position - triangle.vertices[0].position);
    vec3 v1 = vec3(triangle.vertices[2].position - triangle.vertices[0].position);
    vec3 v2 = triangle.hitInfo.position - vec3(triangle.vertices[0].position);

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

vec2 interpolateTexture(vec3 barycentricCoords, Primitive triangle) {
    return triangle.vertices[0].texture * barycentricCoords.x + triangle.vertices[1].texture * barycentricCoords.y + triangle.vertices[2].texture * barycentricCoords.z;
}

float interpolateAlbedo(vec3 barycentricCoords, Primitive triangle) {
    return triangle.vertices[0].albedo * barycentricCoords.x + triangle.vertices[1].albedo * barycentricCoords.y + triangle.vertices[2].albedo * barycentricCoords.z;
}

vec4 interpolateColour(vec3 barycentricCoords, Primitive triangle) {
    bool outlineEnabled = true;

    vec4 colour = triangle.vertices[0].colour * barycentricCoords.x + triangle.vertices[1].colour * barycentricCoords.y + triangle.vertices[2].colour * barycentricCoords.z;
    vec2 textureCoords = interpolateTexture(barycentricCoords, triangle);
    vec4 textureColour = texture(textureSampler, textureCoords);
    if(bool(triangle.vertices[0].textured))
    {
        if((textureCoords.x < 0.05 || textureCoords.x > 1 - 0.05 || textureCoords.y < 0.05 || textureCoords.y > 1 - 0.05) && outlineEnabled)
        {
            colour = vec4(0.0, 0.0, 0.0, 1.0);
        }
        return (colour + textureColour) / 2;
    }
    else
    {
        return colour;
    }
}

vec3 calculateNormal(Primitive triangle) {
    vec3 edge1 = vec3(triangle.vertices[1].position - triangle.vertices[0].position);
    vec3 edge2 = vec3(triangle.vertices[2].position - triangle.vertices[0].position);
    vec3 normal = normalize(cross(edge1, edge2));
    return normal;
}

void getPrimitive(  int i,
                    out Primitive triangle)
{
    triangle.vertices[0] = vertices[indices[i]];
    triangle.vertices[1] = vertices[indices[i+1]];
    triangle.vertices[2] = vertices[indices[i+2]];
}

void rayTrace(out Ray ray)
{
    int bounces = -1;
    float albedos[MAX_BOUNCES];
    vec4 baseColours[MAX_BOUNCES];
    vec4 colours[MAX_BOUNCES];

    for(int i = 0; i <= MAX_BOUNCES; i++)
    {
        //Intersection information
        vec3 closestIntersection = vec3(10000000000.0);
        bool intersectsAny = false;
        vec4 baseColour = vec4(1.0);
        float albedo = 0;
        vec3 normal = vec3(1.0);

        //Primitive information
        Primitive triangle;

        //For each primitive
        for(int j = 0; j < numIndices; j += 3)
        {
            //Get primitive info
            getPrimitive(j, triangle);

            //Checks for intersection
            intersectRayTriangle(ray, triangle);

            //If this is the closest intersection, then update values
            if (triangle.hitInfo.didHit && length(triangle.hitInfo.position) < length(closestIntersection))
            {
                intersectsAny = true;
                closestIntersection = triangle.hitInfo.position;

                vec3 barycentricCoords = barycentric(triangle);
                albedo = interpolateAlbedo(barycentricCoords, triangle);
                normal = calculateNormal(triangle);
                baseColour = interpolateColour(barycentricCoords, triangle);
            }
        }

        //Enter the intersecting surface info into arrays
        bounces = i;

        albedos[i] = albedo;
        baseColours[i] = baseColour;

        //Update ray information for following bounces
        ray.direction = reflect(ray.direction, normal);
        ray.origin = closestIntersection + normal * 0.00001;


        //If the ray didnt hit a surface, then break the loop
        if(!intersectsAny)
        {
            break;
        }
    }

    //Calculate the colour of the pixel based on the full reflection data
    colours[bounces] = baseColours[bounces];

    for(int i = bounces - 1; i >= 0; i--)
    {
        colours[i] = albedos[i] * colours[i + 1] + (1 - albedos[i]) * baseColours[i];
    }

    ray.colour = colours[0];
}

void main()
{
    // Initialise ray properties
    Ray ray;
    ray.origin = vec3(0.0);
    ray.direction = vec3(screen.x, screen.y, 1.0);
    ray.colour = vec4(1.0);

    // Calculate pixel colour
    rayTrace(ray);

    // Draw colour
	FragColor = ray.colour;
}