//
// Created by kshitij on 28/6/25.
//

#pragma once

#include <glm/glm.hpp>

// Aligned to 16 bytes
struct PointLight {
    glm::vec3 position      = glm::vec3(0.0f);
    float constant          = 1.0f;

    glm::vec3 ambient       = glm::vec3(0.05f);
    float linear            = 0.09f;

    glm::vec3 diffuse       = glm::vec3(1.0f);
    float quadratic         = 0.032f;

    glm::vec3 specular      = glm::vec3(1.0f);
    float pad = 0.0f;
};
