#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "Components/Rendering/Shader.hpp"

struct Material {
    entt::entity shader = entt::null;

    bool initialized = false;
};
