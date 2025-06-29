#include "Hierarchy.hpp"
#include "Components/Constants.hpp"
#include "Components/Primitives/Transform.hpp"

#include <iostream>

namespace Hierarchy {
    void Show(entt::registry &registry){
        // Anything that has a transform is part of the Hierarchy
        auto &[entities, selectedID, selectedEntity] = registry.ctx().get<Hierarchy>();
        const auto transformView = registry.view<Transform>();

        // TODO: Adding to vector each frame, might be a better way
        // Clear before starting
        entities.clear();

        for(auto entity : std::views::reverse(transformView)){
            entities.emplace_back(entity);
        }

        const auto &c = registry.ctx().get<Constants>();

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Appearing);
        const int width = c.WINDOW_WIDTH - (c.SCENE_WIDTH + c.SCENE_X);
        const int height = c.SCENE_HEIGHT;
        ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Appearing);
        ImGui::Begin("Hierarchy");

        bool scrollToSelection = false;
        if (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive()) {
            if (ImGui::IsKeyPressed(ImGuiKey_UpArrow) && selectedID > 0) {
                selectedID--;
                scrollToSelection = true;
            }
            if (ImGui::IsKeyPressed(ImGuiKey_DownArrow) && selectedID < entities.size() - 1) {
                selectedID++;
                scrollToSelection = true;
            }
        }

        for (int i = 0; i < entities.size(); ++i) {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth;

            if (i == selectedID)
                flags |= ImGuiTreeNodeFlags_Selected;

            ImGui::PushID(i);

            if (auto &transform = registry.get<Transform>(entities[i]);
                // Space for padding
                ImGui::Selectable( (" " + transform.name).c_str(), i == selectedID, ImGuiSelectableFlags_SpanAllColumns)) {
                selectedID = i;
            }

            if (i == selectedID) {
                // Only scroll if we're navigating with keys or if it's not visible
                if (scrollToSelection) {
                    ImGui::SetScrollHereY(0.5f); // Center selected item
                    scrollToSelection = false;
                }

                // Optional highlight
                ImVec2 min = ImGui::GetItemRectMin();
                ImVec2 max = ImGui::GetItemRectMax();
                ImGui::GetWindowDrawList()->AddRect(min, max, IM_COL32(0, 215, 255, 255), 4.0f, 0, 2.0f);
            }

            ImGui::PopID();

            // Right-click context menu
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Rename")) { }
                if (ImGui::MenuItem("Delete")) { }
                ImGui::EndPopup();
            }
        }
        ImGui::End();

        if(selectedID != -1){
            selectedEntity = entities[selectedID];
        }

    }

}
