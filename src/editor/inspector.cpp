#include "inspector.hpp"

using namespace Editor;

Inspector::Inspector() {
    // Empty Constructor
}

void Inspector::Show(SimObject::Entity& entity, ImGuizmo::OPERATION &operation, ImGuizmo::MODE &mode){
    ImGui::SetNextWindowPos(ImVec2(WIDTH - inspectorWidth, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(inspectorWidth, HEIGHT), ImGuiCond_Always);
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
        entity.GUI();
    }

    ImGui::End();
}
