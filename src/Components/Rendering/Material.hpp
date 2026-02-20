#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "Components/Rendering/Shader.hpp"

struct Material {
    entt::entity shader = entt::null;

    glm::vec3 diffuseColor = glm::vec3(1);
    glm::vec3 specularColor = glm::vec3(1);
    float shininess = 16;

    bool initialized = false;
};
