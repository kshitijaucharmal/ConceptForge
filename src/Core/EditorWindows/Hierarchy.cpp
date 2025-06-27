#include "Hierarchy.hpp"
#include "Components/Constants.hpp"
#include "Components/Primitives/Transform.hpp"

#include <iostream>

namespace Hierarchy {
    void Show(entt::registry &registry){
        // Anything that has a transform is part of the Hierarchy
        auto &hierarchy = registry.ctx().get<Hierarchy>();
        auto transformView = registry.view<Transform>();

        // TODO: Adding to vector each frame, might be a better way
        // Clear before starting
        hierarchy.entities.clear();

        for(auto entity : transformView){
            hierarchy.entities.push_back(entity);
        }

        auto &c = registry.ctx().get<Constants>();

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        int width = c.WINDOW_WIDTH - (c.SCENE_WIDTH + c.SCENE_X);
        int height = c.SCENE_HEIGHT;
        ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
        ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        bool scrollToSelection = false;
        if (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive()) {
            if (ImGui::IsKeyPressed(ImGuiKey_UpArrow) && hierarchy.selectedID > 0) {
                hierarchy.selectedID--;
                scrollToSelection = true;
            }
            if (ImGui::IsKeyPressed(ImGuiKey_DownArrow) && hierarchy.selectedID < hierarchy.entities.size() - 1) {
                hierarchy.selectedID++;
                scrollToSelection = true;
            }
        }

        for (int i = 0; i < hierarchy.entities.size(); ++i) {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth;

            if (i == hierarchy.selectedID)
                flags |= ImGuiTreeNodeFlags_Selected;

            ImGui::PushID(i);

            auto &transform = registry.get<Transform>(hierarchy.entities[i]);
            if (ImGui::Selectable(transform.name.c_str(), i == hierarchy.selectedID, ImGuiSelectableFlags_SpanAllColumns)) {
                hierarchy.selectedID = i;
            }

            if (i == hierarchy.selectedID) {
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

        if(hierarchy.selectedID != -1){
            hierarchy.selectedEntity = hierarchy.entities[hierarchy.selectedID];
        }

    }

}
