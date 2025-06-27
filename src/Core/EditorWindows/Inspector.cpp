#include "Inspector.hpp"
#include "Components/Primitives/Transform.hpp"
#include "Components/Rendering/GizmoControls.hpp"
#include "Components/Constants.hpp"

#include "Core/EditorWindows/Hierarchy.hpp"

#include "imgui.h"
#include "ImGuizmo.h"
#include <glm/gtc/type_ptr.hpp>

namespace Inspector {
    void Show(entt::registry &registry) {
        auto &selectedObject = registry.ctx().get<Hierarchy::Hierarchy>().selectedEntity;

        auto &constants = registry.ctx().get<Constants>();
        auto &gc = registry.ctx().get<GizmoControls>();

        ImGui::SetNextWindowPos(ImVec2(constants.SCENE_X + constants.SCENE_WIDTH, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(constants.WINDOW_WIDTH - (constants.SCENE_WIDTH + constants.SCENE_X), constants.WINDOW_HEIGHT), ImGuiCond_Always);
        ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        if (ImGui::CollapsingHeader("Gizmo Controls", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::RadioButton("Translate", gc.operation == ImGuizmo::TRANSLATE)) gc.operation = ImGuizmo::TRANSLATE;
            ImGui::SameLine();
            if (ImGui::RadioButton("Rotate", gc.operation == ImGuizmo::ROTATE)) gc.operation = ImGuizmo::ROTATE;
            ImGui::SameLine();
            if (ImGui::RadioButton("Scale", gc.operation == ImGuizmo::SCALE)) gc.operation = ImGuizmo::SCALE;

            if (ImGui::RadioButton("Local", gc.mode == ImGuizmo::LOCAL)) gc.mode = ImGuizmo::LOCAL;
            ImGui::SameLine();
            if (ImGui::RadioButton("Global", gc.mode == ImGuizmo::WORLD)) gc.mode = ImGuizmo::WORLD;
        }

        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen) && selectedObject != entt::null) {

            auto &transform = registry.get<Transform>(selectedObject);

            ImGui::DragFloat3("Position", glm::value_ptr(transform.position), 0.01,  -10.0, 10.0);
            ImGui::DragFloat3("Rotation", glm::value_ptr(transform.rotation), .05,  -180.0, 180.0);
            ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale), 0.1,  0.0, 100.0);
        }

        ImGui::End();

    }
}
