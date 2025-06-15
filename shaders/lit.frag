#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

float ambientStrength = 0.1;
float specularStrength = 0.5;

void main() {

  // Ambient light
  vec3 ambient = ambientStrength * lightColor;

  // Diffuse Light (Based on normal)
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  // Specular Light (Based on view)
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;

  vec3 result = (diffuse + ambient + specular) * objectColor;
  FragColor = vec4(result, 1.0);
}
