//
// Created by kshitij on 28/6/25.
//

#pragma once

#include <glm/glm.hpp>

// Aligned to 16 bytes
struct alignas(16) DirectionalLight {
    glm::vec3 direction = glm::vec3(0.0f);
    float pad1          = 0.0f;

    glm::vec3 ambient   = glm::vec3(0.5f);
    float pad2          = 0.0f;

    glm::vec3 diffuse   = glm::vec3(0.8f);
    float pad3          = 0.0f;

    glm::vec3 specular  = glm::vec3(0.1f);
    float pad4          = 0.0f;

    // Shadow Mapping
    /// projection * view matrix for this light
    glm::mat4 lightSpaceMatrix = glm::mat4(1.0f);

    /// Which Layer in GL_TEXTURE_2D_Array this belongs to. -1 for no shadows
    int shadowMapIndex  = -1;
    float shadowBias    = 0.002f;
    int castShadows     = 1;
    int hardShadows     = 1;
};
