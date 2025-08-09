
#pragma once

#include <entt/entt.hpp>
#include <glad/glad.h>

#include "Components/Rendering/Material.hpp"
#include "Systems/Rendering/ShaderSystem.hpp"

namespace MaterialSystem {
    using FallbackTexture = unsigned int;
    // For ALL Materials
    void InitFallbackTexture(entt::registry &registry);

    // void ApplyShaderParameters(Material &material);
}
