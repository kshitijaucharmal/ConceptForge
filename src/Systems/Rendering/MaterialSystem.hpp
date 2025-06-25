
#pragma once

#include <entt/entt.hpp>
#include <glad/glad.h>

#include "Components/Rendering/Material.hpp"
#include "Systems/Rendering/ShaderSystem.hpp"

namespace MaterialSystem {
    // For ALL Materials
    void InitMaterials(entt::registry &registry);
    void InitWhiteTexture(entt::registry &registry);

    // void ApplyShaderParameters(Material &material);
}
