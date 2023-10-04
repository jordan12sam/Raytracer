#version 460 core

in vec2 screen;
out vec4 FragColor;

uniform int indices[1200];
uniform int numVertices;
uniform int numIndices;
uniform vec4 lightPos;
uniform sampler2D textureSampler;

#define MAX_BOUNCES 10

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

struct Intersection {
    bool exists;
    vec3 position;
    int primitiveIndex;
};

struct Primitive {
    Intersection intersection;
    vec3 normal;
    Vertex vertices[3];
};

struct Ray {
    vec3 origin;
    vec3 direction;
    vec4 colour;
};

void getPrimitive(  int i, out Primitive triangle) {
    triangle.vertices[0] = vertices[indices[i]];
    triangle.vertices[1] = vertices[indices[i+1]];
    triangle.vertices[2] = vertices[indices[i+2]];
}

vec3 calculateNormal(Primitive triangle) {
    vec3 edge1 = vec3(triangle.vertices[1].position - triangle.vertices[0].position);
    vec3 edge2 = vec3(triangle.vertices[2].position - triangle.vertices[0].position);
    vec3 normal = normalize(cross(edge1, edge2));
    return normal;
}

vec3 calculateBarycentricCoordinates(Primitive triangle) {
    vec3 v0 = vec3(triangle.vertices[1].position - triangle.vertices[0].position);
    vec3 v1 = vec3(triangle.vertices[2].position - triangle.vertices[0].position);
    vec3 v2 = triangle.intersection.position - vec3(triangle.vertices[0].position);

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

void testRayPrimitiveIntersection(Ray ray, out Primitive triangle) {
    const float EPSILON = 0.000000000000001;

    vec3 edge1 = vec3(triangle.vertices[1].position - triangle.vertices[0].position);
    vec3 edge2 = vec3(triangle.vertices[2].position - triangle.vertices[0].position);
    vec3 h = cross(ray.direction, edge2);
    float a = dot(edge1, h);
    if (a > -EPSILON && a < EPSILON) {
        triangle.intersection.exists = false;
        return;
    }

    float f = 1.0 / a;
    vec3 s = -vec3(triangle.vertices[0].position);
    float u = f * dot(s, h);
    if (u < 0.0 || u > 1.0) {
        triangle.intersection.exists = false;
        return;
    }

    vec3 q = cross(s, edge1);
    float v = f * dot(ray.direction, q);
    if (v < 0.0 || u + v > 1.0) {
        triangle.intersection.exists = false;
        return;
    }

    float t = f * dot(edge2, q);
    if (t > EPSILON) {
        triangle.intersection.position = ray.direction * t;
        triangle.intersection.exists = true;
        return;
    }

    triangle.intersection.exists = false;
}

void rayTrace(out Ray ray) {
    int bounces = -1;
    float albedos[MAX_BOUNCES];
    vec4 baseColours[MAX_BOUNCES];
    vec4 colours[MAX_BOUNCES];

    for(int i = 0; i <= MAX_BOUNCES; i++) {
        Intersection closestIntersection;
        closestIntersection.exists = false;
        closestIntersection.position = vec3(9999999.0);
        vec4 baseColour = vec4(0.0);
        float albedo = 0;
        vec3 normal = vec3(1.0);

        Primitive triangle;
        for(int j = 0; j < numIndices; j += 3) {
            getPrimitive(j, triangle);
            testRayPrimitiveIntersection(ray, triangle);
            if (triangle.intersection.exists && length(triangle.intersection.position) < length(closestIntersection.position)) {
                closestIntersection.exists = true;
                closestIntersection.position = triangle.intersection.position;
                closestIntersection.primitiveIndex = j;
            }
        }

        if(closestIntersection.exists) {
            getPrimitive(closestIntersection.primitiveIndex, triangle);
            vec3 barycentricCoords = calculateBarycentricCoordinates(triangle);
            albedo = interpolateAlbedo(barycentricCoords, triangle);
            normal = calculateNormal(triangle);
            baseColour = interpolateColour(barycentricCoords, triangle);

            vec3 lightDir = normalize(closestIntersection.position - lightPos.xyz);
            vec3 viewDir = normalize(closestIntersection.position - ray.origin);

            float ambientStrength = 1.0;
            float diffuseStrength = 0.5;
            float specularStrength = 0.3;
            float shininess = 32.0;

            vec3 ambientColor = vec3(0.1, 0.1, 0.1);
            vec3 lightColor = vec3(1.0, 1.0, 1.0);

            float diff = max(dot(normal, lightDir), 0.0);
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

            vec3 ambient = ambientStrength * ambientColor;
            vec3 diffuse = diffuseStrength * diff * lightColor;
            vec3 specular = specularStrength * spec * lightColor;

            vec3 finalColor = baseColour.rgb * (ambient + diffuse + specular);

            bounces = i;
            albedos[i] = albedo;
            baseColours[i] = vec4(finalColor, 1.0);

            ray.direction = normalize(reflect(ray.direction, normal));
            ray.origin = closestIntersection.position + normal * 0.00001;
        }
        else {
            break;
        }
    }

    colours[bounces] = baseColours[bounces];
    for(int i = bounces - 1; i >= 0; i--) {
        colours[i] = albedos[i] * colours[i + 1] + (1 - albedos[i]) * baseColours[i];
    }

    ray.colour = colours[0];
}

void main()
{
    Ray ray;
    ray.origin = vec3(0.0);
    ray.direction = normalize(vec3(screen.x, screen.y, 1.0));
    ray.colour = vec4(1.0);

    rayTrace(ray);
    
	FragColor = ray.colour;
}