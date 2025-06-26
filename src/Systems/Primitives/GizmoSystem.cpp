
#include "GizmoSystem.hpp"
#include "Components/Constants.hpp"
#include "Components/Primitives/ActiveObject.hpp"
#include "Components/Primitives/Transform.hpp"

#include "Systems/CameraSystem.hpp"
#include "Systems/SimObjectSystem.hpp"

namespace GizmoSystem {

    void Render(entt::registry &registry) {
        auto constants = registry.ctx().get<Constants>();

        // Default
        ImGuizmo::OPERATION gizmoOperation = ImGuizmo::TRANSLATE;
        ImGuizmo::MODE gizmoMode = ImGuizmo::LOCAL;

        ImGui::Begin("Hello");
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(0, 0, (float)constants.WINDOW_WIDTH, (float)constants.WINDOW_HEIGHT);
        ImGuizmo::BeginFrame();

        // Get ActiveCamera
        auto &camEntity = registry.ctx().get<ActiveCamera>().camera;
        auto &activeCamera = registry.get<Camera>(camEntity);

        auto &activeObject = registry.ctx().get<ActiveObject>().entity;
        auto &transform = registry.get<Transform>(activeObject);

        auto model = SimObject::ComposeTransform(transform);
        ImGuizmo::Manipulate(glm::value_ptr(activeCamera.view),
                             glm::value_ptr(activeCamera.projection),
                             gizmoOperation,
                             gizmoMode,
                             glm::value_ptr(model),
                             nullptr,
                             nullptr);

        // Modify camera
        glm::mat4 cameraMatrix = glm::inverse(activeCamera.view);
        glm::vec3 position = glm::vec3(cameraMatrix[3]);
        glm::vec3 forward  = -glm::vec3(cameraMatrix[2]);
        glm::vec3 up       = glm::vec3(cameraMatrix[1]);

        SetTransform(activeCamera, position, up);
        LookAt(activeCamera, position + forward);

        activeCamera.view = GetViewMatrix(activeCamera);

        // If the gizmo was used, decompose the result and update
        if (ImGuizmo::IsUsing()) {
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::quat rotQuat;

            glm::decompose(model, transform.scale, rotQuat, transform.position, skew, perspective);
            transform.rotation = glm::degrees(glm::eulerAngles(rotQuat)); // Convert to Euler angles
        }
        ImGui::End();

    }
}
