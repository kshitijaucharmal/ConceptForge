
#include "GizmoSystem.hpp"
#include "Components/Constants.hpp"
#include "Components/Primitives/Transform.hpp"
#include "Components/Rendering/GizmoControls.hpp"

#include "Core/EditorWindows/Hierarchy.hpp"

#include "Systems/CameraSystem.hpp"
#include "Systems/SimObjectSystem.hpp"

namespace GizmoSystem {
    void Render(entt::registry &registry) {
        auto activeObject = registry.ctx().get<Hierarchy::Hierarchy>().selectedEntity;
        // Return if nothing selected
        if(activeObject == entt::null) return;

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
        auto &gizmoControls = registry.ctx().get<GizmoControls>();

        ImVec2 scenePos = ImGui::GetCursorScreenPos();
        ImVec2 sceneSize = ImGui::GetContentRegionAvail();
        ImGui::PushClipRect(scenePos, ImVec2(scenePos.x + sceneSize.x, scenePos.y + sceneSize.y), true);
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
        ImGuizmo::SetRect(scenePos.x, scenePos.y, sceneSize.x, sceneSize.y);
        ImGuizmo::BeginFrame();

        // Get ActiveCamera
        auto &camEntity = registry.ctx().get<ActiveCamera>().camera;
        auto &activeCamera = registry.get<Camera>(camEntity);
        auto &activeCameraTransform = registry.get<Transform>(camEntity);

        auto &transform = registry.get<Transform>(activeObject);

        auto model = SimObject::ComposeTransform(transform);
        ImGuizmo::Manipulate(glm::value_ptr(activeCamera.view),
                             glm::value_ptr(activeCamera.projection),
                             gizmoControls.operation,
                             gizmoControls.mode,
                             glm::value_ptr(model),
                             nullptr,
                             nullptr);

        activeCamera.view = CameraSystem::GetViewMatrix(activeCamera, activeCameraTransform);

        // If the gizmo was used, decompose the result and update
        if (ImGuizmo::IsUsing()) {
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::quat rotQuat;

            glm::decompose(model, transform.scale, rotQuat, transform.position, skew, perspective);
            transform.rotation = glm::normalize(rotQuat);
        }

        ImGui::PopClipRect();
        ImGui::End(); // SceneGizmoOverlay
    }
}
