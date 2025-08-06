
#pragma once

#include <entt/entt.hpp>
#include <glad/glad.h>

#include "Components/Rendering/Material.hpp"
#include "Systems/Rendering/ShaderSystem.hpp"

namespace MaterialSystem {
    using WhiteTexture = unsigned int;
    // For ALL Materials
    void InitWhiteTexture(entt::registry &registry);

    // void ApplyShaderParameters(Material &material);
}
