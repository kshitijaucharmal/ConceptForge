//
// Created by kshitij on 28/6/25.
//

#pragma once

#include <glm/glm.hpp>

// Aligned to 16 bytes
struct alignas(16) DirectionalLight {
    glm::vec3 direction = glm::vec3(0.0f);
    float pad1          = 0.0f;

    glm::vec3 ambient   = glm::vec3(0.05f);
    float pad2          = 0.0f;

    glm::vec3 diffuse   = glm::vec3(0.1f);
    float pad3          = 0.0f;

    glm::vec3 specular  = glm::vec3(0.1f);
    float pad4          = 0.0f;
};
