
#include "GizmoSystem.hpp"

#include <imgui_internal.h>
#include <Core/GameState.hpp>

#include "../../Components/Physics/Rigidbody.hpp"
#include "../../Core/Physics/PhysicsSystem.hpp"
#include "Components/Constants.hpp"
#include "Components/Primitives/Transform.hpp"
#include "Components/Rendering/GizmoControls.hpp"

#include "Core/EditorWindows/Hierarchy.hpp"

#include "Systems/CameraSystem.hpp"
#include "Systems/SimObjectSystem.hpp"

namespace GizmoSystem {
    void Render(entt::registry &registry) {
        auto &gameState = registry.ctx().get<GameState>();
        if (gameState.isPlaying) return;

        const auto activeObject = registry.ctx().get<Hierarchy::Hierarchy>().selectedEntity;
        if(activeObject == entt::null) return;

        ImGui::Begin("SceneGizmoOverlay", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);

        const auto &[operation, mode] = registry.ctx().get<GizmoControls>();
        const ImVec2 scenePos = ImGui::GetCursorScreenPos();
        const ImVec2 sceneSize = ImGui::GetContentRegionAvail();

        ImGui::PushClipRect(scenePos, ImVec2(scenePos.x + sceneSize.x, scenePos.y + sceneSize.y), true);
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
        ImGuizmo::SetRect(scenePos.x, scenePos.y, sceneSize.x, sceneSize.y);
        ImGuizmo::BeginFrame();

        const auto &camEntity = registry.ctx().get<ActiveCamera>().camera;
        auto &activeCamera = registry.get<Camera>(camEntity);
        auto &activeCameraTransform = registry.get<Transform>(camEntity);

        auto &transform = registry.get<Transform>(activeObject);
        glm::mat4 worldModel = transform.model; // Use the pre-calculated world matrix

        ImGuizmo::Manipulate(glm::value_ptr(activeCamera.view),
                             glm::value_ptr(activeCamera.projection),
                             operation, mode,
                             glm::value_ptr(worldModel),
                             nullptr, nullptr);

        // ONLY update components if the user is actually moving the gizmo
        if (ImGuizmo::IsUsing()) {
            glm::mat4 localMatrix = worldModel;

            if (transform.parent != entt::null) {
                auto& parentTransform = registry.get<Transform>(transform.parent);
                // Bring world back to local: Local = Inverse(ParentWorld) * World
                localMatrix = glm::inverse(parentTransform.model) * worldModel;
            }

            glm::vec3 skew;
            glm::vec4 perspective;
            glm::quat rotQuat;

            // Now decompose the localMatrix (which is correct for both parented and unparented objects)
            glm::decompose(localMatrix, transform.scale, rotQuat, transform.position, skew, perspective);
            transform.rotation = glm::normalize(rotQuat);
            transform.eulerAngles = glm::degrees(glm::eulerAngles(transform.rotation));
        }

        activeCamera.view = CameraSystem::GetViewMatrix(activeCamera, activeCameraTransform);

        ImGui::PopClipRect();
        ImGui::End();
    }
}
