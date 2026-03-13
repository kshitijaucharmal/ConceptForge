#version 430 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};

struct DirLight {
    vec3 direction;     float pad1;
    vec3 ambient;       float pad2;
    vec3 diffuse;       float pad3;
    vec3 specular;      float pad4;
    mat4 lightSpaceMatrix;
    int shadowMapIndex; float shadowBias;
    int castShadows;    int hardShadows;
};

struct PointLight {
    vec3 position;  float constant;
    vec3 ambient;   float linear;
    vec3 diffuse;   float quadratic;
    vec3 specular;  float pad;
};

layout(std430, binding = 1) buffer PointLights { PointLight pointLights[]; };
layout(std430, binding = 2) buffer DirLightsBuffer { DirLight dirLights[]; };

uniform sampler2DArray shadowMaps;
uniform Material material;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// --- Helper: Convert sRGB texture data to Linear Space ---
vec3 srgb_to_linear(vec3 srgb) {
    return mix(
    srgb * (1.0 / 12.92),
    pow(max((srgb + 0.055) / 1.055, 0.0), vec3(2.4)),
    step(vec3(0.04045), srgb)
    );
}

// --- Shadow Calculation ---
float random(vec3 seed, int i) {
    vec4 seed4 = vec4(seed, i);
    float dot_product = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));
    return fract(sin(dot_product) * 43758.5453);
}

float CalculateShadow(vec4 fragPosLightSpace, int layer, float bias, int hardShadows) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if (projCoords.z > 1.0) return 0.0;

    if (hardShadows == 1) {
        float closestDepth = texture(shadowMaps, vec3(projCoords.xy, layer)).r;
        return (projCoords.z - bias) > closestDepth ? 1.0 : 0.0;
    }

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMaps, 0).xy;
    for (int x = -2; x <= 2; ++x) {
        for (int y = -2; y <= 2; ++y) {
            float offset = random(gl_FragCoord.xyz, x + y * 5);
            vec2 jitter = vec2(offset) * texelSize;
            float pcfDepth = texture(shadowMaps, vec3(projCoords.xy + (vec2(x,y) + jitter) * texelSize, layer)).r;
            shadow += (projCoords.z - bias) > pcfDepth ? 1.0 : 0.0;
        }
    }
    return shadow / 25.0;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
    vec3 lightDir = normalize(-light.direction);
    float shadow = 0.0;
    if (light.castShadows == 1 && light.shadowMapIndex != -1) {
        vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(fragPos, 1.0);
        shadow = CalculateShadow(fragPosLightSpace, light.shadowMapIndex, light.shadowBias, light.hardShadows);
    }

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    vec4 rawTex = texture(material.texture_diffuse1, TexCoords);
    vec3 texDiffuse = srgb_to_linear(rawTex.rgb);
    vec3 texSpecular = texture(material.texture_specular1, TexCoords).rgb;

    vec3 finalDiff = texDiffuse * material.diffuseColor;
    vec3 finalSpec = texSpecular * material.specularColor;

    vec3 ambient  = light.ambient  * finalDiff * 0.25; // Keeping your tuned balance [cite: 77]
    vec3 diffuse  = light.diffuse  * diff * finalDiff * (1.0 - shadow);
    vec3 specular = light.specular * spec * finalSpec * (1.0 - shadow);

    return ambient + diffuse * 1.4 + specular * 0.4; // Keeping tuned balance
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / max(light.constant + light.linear * distance + light.quadratic * distance * distance, 0.0001);

    vec3 texDiffuse = srgb_to_linear(texture(material.texture_diffuse1, TexCoords).rgb);
    vec3 texSpecular = texture(material.texture_specular1, TexCoords).rgb;

    vec3 finalDiff = texDiffuse * material.diffuseColor;
    vec3 finalSpec = texSpecular * material.specularColor;

    vec3 ambient  = light.ambient  * finalDiff * 0.2;
    vec3 diffuse  = light.diffuse  * diff * finalDiff;
    vec3 specular = light.specular * spec * finalSpec;

    return (ambient + diffuse * 1.4 + specular * 0.4) * attenuation; // [cite: 88]
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);

    for (int i = 0; i < dirLights.length(); i++)
    result += CalcDirLight(dirLights[i], norm, viewDir, FragPos);
    for (int i = 0; i < pointLights.length(); i++)
    result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    // Output raw Linear color, gamma corrected
    FragColor = vec4(result, 1.0);
}