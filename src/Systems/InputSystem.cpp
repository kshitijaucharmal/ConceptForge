//
// Created by kshitij on 29/6/25.
//

#include "InputSystem.hpp"

#include <imgui.h>
#include <Components/Camera.hpp>
#include <Components/Time.hpp>
#include <Systems/CameraSystem.hpp>

#include "Components/Rendering/GizmoControls.hpp"
#include "Core/EditorWindows/Hierarchy.hpp"
#include "Rendering/PickingPassSystem.hpp"

namespace InputSystem {
    void ProcessInput(entt::registry &registry, GLFWwindow *window) {
        const auto deltaTime = registry.ctx().get<Time>().deltaTime;
        auto &constants = registry.ctx().get<Constants>();

        // For Picking (Selecting objects)
        if (constants.LeftMouseCaptured)
        {
            PickingPassSystem::BindFramebuffer(registry);
            PickingPassSystem::Render(registry);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            unsigned char data[4];
            const auto &consts = registry.ctx().get<Constants>();

            glReadPixels(consts.mouseX, constants.WINDOW_HEIGHT - consts.mouseY,1,1, GL_RGBA, GL_UNSIGNED_BYTE, data);

            int pickedID =
                data[0] +
                data[1] * 256 +
                data[2] * 256*256;
            if (const auto selected = static_cast<entt::entity>(pickedID); registry.valid(selected)) {
                const auto transform = registry.get<Transform>(selected);
                registry.ctx().get<Hierarchy::Hierarchy>().selectedEntity = selected;
            }
            else {
                registry.ctx().get<Hierarchy::Hierarchy>().selectedEntity = entt::null;
            }

            PickingPassSystem::UnbindFramebuffer();

            constants.LeftMouseCaptured = false;
        }

        // Camera
        if (constants.RightMouseCaptured) {
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
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            if (camera.firstMouse) {
                camera.lastX = xpos;
                camera.lastY = ypos;
                camera.firstMouse = false;
            }

            const float xoffset = xpos - camera.lastX;
            const float yoffset = ypos - camera.lastY;

            camera.lastX = xpos;
            camera.lastY = ypos;

            CameraSystem::ProcessMouseMovement(camera, transform, xoffset, yoffset, true);
        }
        else
        {
            auto &[operation, mode] = registry.ctx().get<GizmoControls>();
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) operation = ImGuizmo::TRANSLATE;
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) operation = ImGuizmo::ROTATE;
            if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) operation = ImGuizmo::SCALE;
        }
    }
}
