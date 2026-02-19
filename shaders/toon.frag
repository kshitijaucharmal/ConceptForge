#version 430 core

struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
  vec3 diffuseColor;
  vec3 specularColor;
  float shininess;
};

struct DirLight {
  vec3 direction; float pad1;
  vec3 ambient;   float pad2;
  vec3 diffuse;   float pad3;
  vec3 specular;  float pad4;
};

struct PointLight {
  vec3 position;  float constant;
  vec3 ambient;   float linear;
  vec3 diffuse;   float quadratic;
  vec3 specular;  float pad;
};

layout(std430, binding = 1) buffer PointLights { PointLight pointLights[]; };
layout(std430, binding = 2) buffer DirLightsBuffer { DirLight dirLights[]; };

uniform Material material;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// Toon shading settings
const float levels = 4.0; // Number of shading bands
const float scaleFactor = 1.0 / levels;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 result = vec3(0.0);

  for(int i = 0; i < dirLights.length(); i++){
    result += CalcDirLight(dirLights[i], norm, viewDir);
  }
  for(int i = 0; i < pointLights.length(); i++){
    result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
  }

  FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(-light.direction);

  // --- Toon Diffuse ---
  float dotProduct = dot(normal, lightDir);
  float intensity = max(dotProduct, 0.0);
  // Quantize the intensity into steps
  float level = floor(intensity * levels);
  float diff = level * scaleFactor;

  // --- Toon Specular ---
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float specIntensity = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
  // Sharp cut-off for specular highlight
  float spec = specIntensity > 0.5 ? 1.0 : 0.0;

  vec3 texColor = vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 specTex  = vec3(texture(material.texture_specular1, TexCoords));

  vec3 ambient  = light.ambient  * texColor;
  vec3 diffuse  = light.diffuse  * diff * texColor;
  vec3 specular = light.specular * spec * specTex;

  return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = normalize(light.position - fragPos);

  // --- Toon Diffuse ---
  float intensity = max(dot(normal, lightDir), 0.0);
  float diff = floor(intensity * levels) * scaleFactor;

  // --- Toon Specular ---
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float specIntensity = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
  float spec = specIntensity > 0.5 ? 1.0 : 0.0;

  // Attenuation (Keep this smooth or the bands will "wobble" oddly)
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / max(light.constant + light.linear * distance + light.quadratic * (distance * distance), 0.0001);

  vec3 texColor = vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 specTex  = vec3(texture(material.texture_specular1, TexCoords));

  vec3 ambient  = light.ambient  * texColor;
  vec3 diffuse  = light.diffuse  * diff * texColor;
  vec3 specular = light.specular * spec * specTex;

  return (ambient + diffuse + specular) * attenuation;
}