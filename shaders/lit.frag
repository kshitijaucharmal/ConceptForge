#version 430 core

// --------------------------------------------
struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;

  vec3 diffuseColor;
  vec3 specularColor;
  float shininess;
};
// --------------------------------------------
struct DirLight {
  vec3 direction;
  float pad1;

  vec3 ambient;
  float pad2;

  vec3 diffuse;
  float pad3;

  vec3 specular;
  float pad4;
};
// --------------------------------------------
struct PointLight {
  vec3 position;
  float constant;

  vec3 ambient;
  float linear;

  vec3 diffuse;
  float quadratic;

  vec3 specular;
  float pad; // Declared to keep 16 byte aligned (vec3 + float = 4 float = 16 bytes)
};

// SSBOs -------------------------------------
layout(std430, binding = 1) buffer PointLights {
  PointLight pointLights[];
};
layout(std430, binding = 2) buffer DirLightsBuffer {
  DirLight dirLights[];
};

// --------------------------------------------

uniform Material material;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth) {
  float z = depth * 2.0 - 1.0; // back to NDC
  return (2.0 * near * far) / (far + near - z * (far - near));
}

// Functions
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
  // properties
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);

  vec3 result = vec3(0.0);

  // phase 1: Directional lighting
  for(int i = 0; i < dirLights.length(); i++){
    result += CalcDirLight(dirLights[i], norm, viewDir);
  }
  // phase 2: Point lights
  for(int i = 0; i < pointLights.length(); i++){
    result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
  }
  // phase 3: Spot light
  // result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

  FragColor = vec4(result, 1.0);

//  float depth = LinearizeDepth(gl_FragCoord.z) / far;
//  FragColor = vec4(vec3(depth), 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(-light.direction);

  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);

  // specular shading
  // Not using this
  vec3 reflectDir = reflect(-lightDir, normal);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

  // combine results
  vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
  return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = normalize(light.position - fragPos);

  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);

  // specular shading
  // Not using this
  vec3 reflectDir = reflect(-lightDir, normal);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

  // attenuation
  float distance    = length(light.position - fragPos);
  float attenuation = 1.0 / max(light.constant + light.linear * distance + light.quadratic * (distance * distance), 0.0001);

  // combine results
  vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

  // Apply attenuation
  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;

  return (ambient + diffuse + specular);
}
