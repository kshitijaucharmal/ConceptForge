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
        const auto &selectedObject = registry.ctx().get<Hierarchy::Hierarchy>().selectedEntity;

        const auto &constants = registry.ctx().get<Constants>();
        auto &[operation, mode] = registry.ctx().get<GizmoControls>();

        ImGui::SetNextWindowPos(ImVec2(constants.SCENE_X + constants.SCENE_WIDTH, 0), ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(ImVec2(constants.WINDOW_WIDTH - (constants.SCENE_WIDTH + constants.SCENE_X), constants.WINDOW_HEIGHT), ImGuiCond_Appearing);
        ImGui::Begin("Inspector");

        if (ImGui::CollapsingHeader("Gizmo Controls", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::RadioButton("Translate", operation == ImGuizmo::TRANSLATE)) operation = ImGuizmo::TRANSLATE;
            ImGui::SameLine();
            if (ImGui::RadioButton("Rotate", operation == ImGuizmo::ROTATE)) operation = ImGuizmo::ROTATE;
            ImGui::SameLine();
            if (ImGui::RadioButton("Scale", operation == ImGuizmo::SCALE)) operation = ImGuizmo::SCALE;

            if (ImGui::RadioButton("Local", mode == ImGuizmo::LOCAL)) mode = ImGuizmo::LOCAL;
            ImGui::SameLine();
            if (ImGui::RadioButton("Global", mode == ImGuizmo::WORLD)) mode = ImGuizmo::WORLD;
        }

        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen) && selectedObject != entt::null) {

            auto &transform = registry.get<Transform>(selectedObject);

            static char nameBuf[128];
            strncpy(nameBuf, transform.name.c_str(), sizeof(nameBuf));
            nameBuf[sizeof(nameBuf) - 1] = '\0';

            if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf))) {
                transform.name = nameBuf;
            }
            ImGui::DragFloat3("Position", glm::value_ptr(transform.position), 0.01,  -1000.0, 1000.0);

            glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(transform.rotation));
            if (ImGui::DragFloat3("Rotation", glm::value_ptr(eulerAngles), .5,  -180.0, 180.0) ){
                transform.rotation = glm::quat(glm::radians(eulerAngles));
            }

            ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale), 0.1,  0.0, 100.0);
        }

        ImGui::End();

    }
}
