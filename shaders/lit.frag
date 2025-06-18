#version 330 core

// --------------------------------------------
struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};
// --------------------------------------------
struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
// --------------------------------------------

uniform Light light;
uniform Material material;

float ambientStrength = 0.1;
float specularStrength = 0.5;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 viewPos;

void main() {
  // Diffuse Light (Based on normal)
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);

  // Specular Light (Based on view)
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  vec3 ambient = light.ambient * material.ambient;
  vec3 diffuse = light.diffuse * (diff * material.diffuse);
  vec3 specular = light.specular * (spec * material.specular);

  vec3 result = diffuse + ambient + specular;
  FragColor = vec4(result, 1.0);
}
