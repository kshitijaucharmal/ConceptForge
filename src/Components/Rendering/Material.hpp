#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "Components/Rendering/Shader.hpp"

struct Material {
    entt::entity shader = entt::null;

    bool initialized = false;

    // Default parameters
    uint whiteTexture = 0;
    uint diffuseMap = 0;
    uint specularMap = 0;

    glm::vec3 diffuseColor = glm::vec3(0.0);
    glm::vec3 specularColor = glm::vec3(0.0);
    float shininess = 32.0f;
};
