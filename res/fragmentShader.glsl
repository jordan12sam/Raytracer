#version 330 core

out vec4 fragColour;
in vec2 vTex;
in vec4 vCol;

vec4 edge = vec4(0.0f, 0.0f, 0.0f, 1.0f);

void main()
{
   if (vTex.x < 0.05 || vTex.x > 0.95 || vTex.y < 0.05 || vTex.y > 0.95){
      fragColour = edge;
   }
   else {
      fragColour = vCol;
   }
}