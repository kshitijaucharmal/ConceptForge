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
    vec3 direction; float pad1;
    vec3 ambient; float pad2;
    vec3 diffuse; float pad3;
    vec3 specular; float pad4;

    mat4 lightSpaceMatrix;

    int shadowMapIndex; float shadowBias;
    int castShadows; float pad6;
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
    float pad;
};

// SSBOs -------------------------------------
layout(std430, binding = 1) buffer PointLights {
    PointLight pointLights[];
};
layout(std430, binding = 2) buffer DirLightsBuffer {
    DirLight dirLights[];
};

// Bound to high unit (e.g., 15) via setInt
uniform sampler2DArray shadowMaps;

// --------------------------------------------

uniform Material material;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// --- Shadow Calculation Function ---
float CalculateShadow(vec4 fragPosLightSpace, int layer, float bias) {
    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // Transform to [0,1] range for texture sampling
    projCoords = projCoords * 0.5 + 0.5;

    // If outside the light's far plane, it's not in shadow
    if(projCoords.z > 1.0) return 0.0;

    // Sample the depth array
    float closestDepth = texture(shadowMaps, vec3(projCoords.xy, layer)).r;
    float currentDepth = projCoords.z;

    // Standard shadow comparison with bias
    // Returns 1.0 if in shadow, 0.0 if lit
    float shadow = (currentDepth - bias) > closestDepth ? 1.0 : 0.0;

    return shadow;
}

// Functions
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 fragPos);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);

    // phase 1: Directional lighting (Shadows included)
    for(int i = 0; i < dirLights.length(); i++){
        result += CalcDirLight(dirLights[i], norm, viewDir, FragPos);
    }

    // phase 2: Point lights (No shadows as per request)
    for(int i = 0; i < pointLights.length(); i++){
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
    vec3 lightDir = normalize(-light.direction);

    // 1. Calculate Shadow
    float shadow = 0.0;
    if (light.castShadows == 1 && light.shadowMapIndex != -1) {
        vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(fragPos, 1.0);
        shadow = CalculateShadow(fragPosLightSpace, light.shadowMapIndex, light.shadowBias);
    }

    // 2. Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // 3. Specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    // 4. Combine results
    vec3 texDiffuse  = vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 texSpecular = vec3(texture(material.texture_specular1, TexCoords));

    vec3 ambient  = light.ambient * texDiffuse;

    // Diffuse and Specular are multiplied by (1.0 - shadow)
    // If shadow is 1.0, these components become 0
    vec3 diffuse  = light.diffuse  * diff * texDiffuse * (1.0 - shadow);
    vec3 specular = light.specular * spec * texSpecular * (1.0 - shadow);

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / max(light.constant + light.linear * distance + light.quadratic * (distance * distance), 0.0001);

    vec3 texDiffuse  = vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 texSpecular = vec3(texture(material.texture_specular1, TexCoords));

    vec3 ambient  = light.ambient  * texDiffuse;
    vec3 diffuse  = light.diffuse  * diff * texDiffuse;
    vec3 specular = light.specular * spec * texSpecular;

    return (ambient + diffuse + specular) * attenuation;
}