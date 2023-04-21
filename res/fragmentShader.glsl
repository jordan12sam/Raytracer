#version 330 core

out vec4 fragColour;
in vec2 vTexCoords;

vec4 fill = vec4(1.0f, 0.5f, 0.2f, 1.0f);
vec4 edge = vec4(0.0f, 0.0f, 0.0f, 1.0f);

void main()
{
   if (vTexCoords.x < 0.05 || vTexCoords.x > 0.95 || vTexCoords.y < 0.05 || vTexCoords.y > 0.95){
      fragColour = edge;
   }
   else {
      fragColour = fill;
   }
}