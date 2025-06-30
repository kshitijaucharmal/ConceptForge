//
// Created by kshitij on 29/6/25.
//

#pragma once

#include <entt/entt.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace InputSystem {
    void ProcessInput(entt::registry &registry, GLFWwindow *window);
}
