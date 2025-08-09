#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float iTime;

struct Material{
  sampler2D texture_diffuse1;
  sampler2D texture_specular2;
};

uniform Material material;

float speed = 5.;

void main() {

  vec4 diffuseColor = texture(material.texture_diffuse1, TexCoord);
  vec4 specularColor = texture(material.texture_specular2, TexCoord);

  // Just combining
  FragColor = diffuseColor + specularColor;
  FragColor = vec4(1., 0, 0, 1);
}
