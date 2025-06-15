#include "hierarchy.hpp"
#include <imgui.h>
#include "constants.hpp"

#include <iostream>

using namespace Editor;

Hierarchy::Hierarchy() { }

void Hierarchy::AddEntity(std::shared_ptr<SimObject::Entity> entity) {
    // TODO: This just gives the id in the name, regardless of type. Change this
    entity->name = std::to_string(entityCount) + ". " + entity->name;
    entities[entityCount] = std::move(entity);
    entityCount++;
}

SimObject::Entity* Hierarchy::GetSelected() {
    if(entities.find(selectedEntityID) != entities.end()) return entities[selectedEntityID].get();
    return nullptr;
}


void Hierarchy::Show() {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(Const::assetBrowserWidth, Const::HEIGHT - Const::consoleHeight), ImGuiCond_Always);
    ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive()) {
        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow) && selectedEntityID > 0) {
            selectedEntityID--;
            scrollToSelection = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_DownArrow) && selectedEntityID < entityCount - 1) {
            selectedEntityID++;
            scrollToSelection = true;
        }
    }

    for (int i = 0; i < entityCount; ++i) {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth;

        if (i == selectedEntityID)
            flags |= ImGuiTreeNodeFlags_Selected;

        ImGui::PushID(i);

        if (ImGui::Selectable(entities[i]->name.c_str(), i == selectedEntityID, ImGuiSelectableFlags_SpanAllColumns)) {
            selectedEntityID = i;
        }

        if (i == selectedEntityID) {
            // Only scroll if we're navigating with keys or if it's not visible
            if (scrollToSelection) {
                ImGui::SetScrollHereY(0.5f); // Center selected item
                scrollToSelection = false;
            }

            // Optional highlight (gold border)
            ImVec2 min = ImGui::GetItemRectMin();
            ImVec2 max = ImGui::GetItemRectMax();
            ImGui::GetWindowDrawList()->AddRect(min, max, IM_COL32(255, 215, 0, 255), 0.0f, 0, 2.0f);
        }

        ImGui::PopID();

        // Right-click context menu
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Rename")) {
                //std::cout << "Rename requested for: " << entities[i]->name << std::endl;
            }
            // WARNING entities contain shared_ptrs so not guaranteed to delete
            if (ImGui::MenuItem("Delete")) {
                //std::cout << "Delete requested for: " << entities[i]->name << std::endl;
            }
            ImGui::EndPopup();
        }
    }

    ImGui::End();
}
