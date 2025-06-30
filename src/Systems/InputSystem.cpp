//
// Created by kshitij on 29/6/25.
//

#include "InputSystem.hpp"

#include <iostream>
#include <Components/Camera.hpp>
#include <Components/Time.hpp>
#include <Systems/CameraSystem.hpp>

namespace InputSystem {
    void ProcessInput(entt::registry &registry, GLFWwindow *window) {
        auto deltaTime = registry.ctx().get<Time>().deltaTime;
        auto &constants = registry.ctx().get<Constants>();

        const auto &activeCam = registry.ctx().get<ActiveCamera>().camera;
        auto &camera = registry.get<Camera>(activeCam);
        auto &transform = registry.get<Transform>(activeCam);

        auto &io = ImGui::GetIO();
        auto dir = glm::vec3(0.0);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) dir.z += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) dir.z -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) dir.x -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) dir.x += 1.0f;
        CameraSystem::ProcessKeyboard(camera, transform, dir, deltaTime);

        // Camera
        if (constants.MouseCaptured) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            if (camera.firstMouse) {
                camera.lastX = xpos;
                camera.lastY = ypos;
                camera.firstMouse = false;
            }

            float xoffset = xpos - camera.lastX;
            float yoffset = ypos - camera.lastY;

            camera.lastX = xpos;
            camera.lastY = ypos;

            CameraSystem::ProcessMouseMovement(camera, transform, xoffset, yoffset, true);
        }
    }
}
