#pragma once

#include <GLFW/glfw3.h>
#include <entt/entt.hpp>

#include "Components/Constants.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace GUISystem {
    using ImGuiDrawQueue = std::vector<std::function<void()>>;

    void InitImGUI(entt::registry &registry, GLFWwindow *window);
    void NewFrame();
    void RenderFrame();
    void PushToDrawQueue(entt::registry &registry, std::function<void()> fn);
    void Destroy();
}
