//
// Created by kshitij on 2/20/26.
//

#ifndef CONCEPTFORGE_LIGHTDEPTHPASSSYSTEM_HPP
#define CONCEPTFORGE_LIGHTDEPTHPASSSYSTEM_HPP

#include "Components/Rendering/Shader.hpp"
#include "entt/entt.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"

namespace LightDepthPassSystem
{
    /// Initialize the Framebuffer
    void Init(entt::registry &registry);
    /// Bind the framebuffer before using
    void BindFramebuffer(entt::registry &registry);
    /// Unbind the framebuffer after using
    void UnbindFramebuffer();
    void Render(entt::registry &registry);
}

#endif //CONCEPTFORGE_LIGHTDEPTHPASSSYSTEM_HPP