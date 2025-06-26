
#include "GizmoSystem.hpp"
#include "Components/Constants.hpp"
#include "Components/Primitives/ActiveObject.hpp"
#include "Components/Primitives/Transform.hpp"
#include "Components/Rendering/GizmoControls.hpp"

#include "Systems/CameraSystem.hpp"
#include "Systems/SimObjectSystem.hpp"

namespace GizmoSystem {

    void Render(entt::registry &registry) {
        ImGui::Begin("SceneGizmoOverlay", nullptr,
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoScrollWithMouse |
                     ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoInputs |          // <--- disables ImGui input but allows gizmo interaction
                     ImGuiWindowFlags_NoBackground);

        auto constants = registry.ctx().get<Constants>();

        // Default
        auto &gizmoControls = registry.ctx().get<GizmoControls>();

        ImVec2 scenePos = ImGui::GetCursorScreenPos();
        ImVec2 sceneSize = ImGui::GetContentRegionAvail();
        ImGui::PushClipRect(scenePos, ImVec2(scenePos.x + sceneSize.x, scenePos.y + sceneSize.y), true);
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
        ImGuizmo::SetRect(scenePos.x, scenePos.y, sceneSize.x, sceneSize.y);

        // ImVec2 windowPos  = ImGui::GetWindowPos();             // Top-left of the "Scene" window
        // ImVec2 contentMin = ImGui::GetWindowContentRegionMin(); // Offset inside window
        // ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
        // ImVec2 sceneRectMin = ImVec2(windowPos.x + contentMin.x, windowPos.y + contentMin.y);
        // ImVec2 sceneSize     = ImVec2(contentMax.x - contentMin.x, contentMax.y - contentMin.y);

        // ImGuizmo::SetRect(sceneRectMin.x, sceneRectMin.y, sceneSize.x, sceneSize.y);
        ImGuizmo::BeginFrame();

        // Get ActiveCamera
        auto &camEntity = registry.ctx().get<ActiveCamera>().camera;
        auto &activeCamera = registry.get<Camera>(camEntity);

        auto &activeObject = registry.ctx().get<ActiveObject>().entity;
        auto &transform = registry.get<Transform>(activeObject);

        auto model = SimObject::ComposeTransform(transform);
        ImGuizmo::Manipulate(glm::value_ptr(activeCamera.view),
                             glm::value_ptr(activeCamera.projection),
                             gizmoControls.operation,
                             gizmoControls.mode,
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

        ImGui::PopClipRect();
        ImGui::End(); // SceneGizmoOverlay
    }
}
