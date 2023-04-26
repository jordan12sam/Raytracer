#version 330 core

out vec4 fragColour;
in vec2 vTex;
in vec4 vCol;

uniform sampler2D uTexture;

vec4 edge = vec4(0.0f, 0.0f, 0.0f, 1.0f);

void main()
{
   vec4 TexCol = texture(uTexture, vTex);
   if (vTex.x < 0.05 || vTex.x > 0.95 || vTex.y < 0.05 || vTex.y > 0.95){
      fragColour = edge;
   }
   else {
      fragColour = vCol * TexCol;
   }
}