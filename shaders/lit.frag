#version 330 core
struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

uniform Material material;

float ambientStrength = 0.1;
float specularStrength = 0.5;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {

  // Ambient light
  vec3 ambient = lightColor * material.ambient;

  // Diffuse Light (Based on normal)
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = lightColor * (diff * material.diffuse);

  // Specular Light (Based on view)
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = lightColor * (material.specular * spec);

  vec3 result = diffuse + ambient + specular;
  FragColor = vec4(result, 1.0);
}
