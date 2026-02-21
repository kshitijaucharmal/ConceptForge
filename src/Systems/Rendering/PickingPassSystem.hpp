//
// Created by kshitij on 2/21/26.
//

#ifndef CONCEPTFORGE_PICKINGPASSSYSTEM_HPP
#define CONCEPTFORGE_PICKINGPASSSYSTEM_HPP
#include "entt/entt.hpp"
#include "glad/glad.h"
#include "glm/detail/func_packing_simd.inl"
#include "Components/Constants.hpp"

namespace PickingPassSystem {
    /// Initialize the Framebuffer
    void Init(entt::registry &registry);
    /// Bind the framebuffer before using
    void BindFramebuffer(entt::registry &registry);
    /// Unbind the framebuffer after using
    void UnbindFramebuffer();
    void Render(entt::registry &registry);
}

#endif //CONCEPTFORGE_PICKINGPASSSYSTEM_HPP