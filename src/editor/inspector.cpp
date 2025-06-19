#include "inspector.hpp"

#include <iostream>

using namespace Editor;

Inspector::Inspector() {
    // Empty Constructor
}

void Inspector::Show(SimObject::Entity *entity, ImGuizmo::OPERATION &operation, ImGuizmo::MODE &mode){
    // Fallback
    SimObject::Entity fallback;
    if(entity == nullptr) entity = &fallback;

    ImGui::SetNextWindowPos(ImVec2(Const::WIDTH - Const::inspectorWidth, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(Const::inspectorWidth, Const::HEIGHT), ImGuiCond_Always);
    ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

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

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {

        // Store previous values to detect changes
        glm::vec3 prevPosition = entity->position;
        glm::vec3 prevRotation = entity->rotation;
        glm::vec3 prevScale = entity->scale;

        ImGui::DragFloat3("Position", glm::value_ptr(entity->position), 0.01,  -10.0, 10.0);
        ImGui::DragFloat3("Rotation", glm::value_ptr(entity->rotation), .05,  -180.0, 180.0);
        ImGui::DragFloat3("Scale", glm::value_ptr(entity->scale), 0.1,  0.0, 100.0);

        // Only update model matrix if values changed
        if (entity->scale != prevScale || entity->rotation != prevRotation || entity->position != prevPosition) {
            entity->UpdateModelMatrix();
        }
    }

    ImGui::End();
}
