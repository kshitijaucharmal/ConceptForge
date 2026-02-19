#pragma once

#include <entt/entt.hpp>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "Components/Rendering/Mesh.hpp"

namespace RenderSystem {
    void Init(entt::registry &registry);
    void Render(entt::registry& registry);
    void ShowSceneTexture(entt::registry &registry, GLFWwindow* window);
    void BindFramebuffer(entt::registry &registry);
    void UnbindFramebuffer();
    void DrawBorder(entt::registry &registry, const glm::mat4 &model, const std::vector<Mesh> &_meshes);
}
