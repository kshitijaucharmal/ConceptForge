#pragma once

#include <entt/entt.hpp>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "Components/Rendering/Mesh.hpp"

/**
 * Render system handles the Main Rendering pass. It draws to a framebuffer which is then Drawn as a SceneTexture
 */
namespace RenderSystem {
    /// Initialize the Framebuffer
    void Init(entt::registry &registry);
    /// Render all objects with Transform, Meshes, and Material to the frambuffer
    void Render(entt::registry& registry);
    /// Draw the Frambuffer as a texture on the screen
    void ShowSceneTexture(entt::registry &registry, GLFWwindow* window);
    /// Bind the framebuffer before using
    void BindFramebuffer(entt::registry &registry);
    /// Unbind the framebuffer after using
    void UnbindFramebuffer();
    /// Helper function to draw outline around the selected object
    void DrawBorder(entt::registry &registry, const glm::mat4 &model, const std::vector<Mesh> &_meshes);
    // Play Pause Buttons
    void ControlBar(entt::registry &registry);
}
