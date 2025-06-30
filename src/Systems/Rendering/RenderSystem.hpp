#pragma once

#include <entt/entt.hpp>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imgui.h"

namespace RenderSystem {
    void Init(entt::registry &registry);
    void Render(entt::registry& registry);
    void ShowSceneTexture(entt::registry &registry, GLFWwindow* window);
    void BindFramebuffer(entt::registry &registry);
    void UnbindFramebuffer();
}
