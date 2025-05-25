#include "object_creation_menu.hpp"

using namespace Editor;

ObjectCreationMenu::ObjectCreationMenu(){

}

void ObjectCreationMenu::Show(){
    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("RightClickMenu");
    }

    if (ImGui::BeginPopupContextWindow("RightClickMenu")) {
        ImGui::Text("     Create");
        if (ImGui::MenuItem("Option 1")) {
            printf("Option 1 selected\n");
        }
        if (ImGui::MenuItem("Option 2")) {
            printf("Option 2 selected\n");
        }
        if (ImGui::MenuItem("Option 3")) {
            printf("Option 3 selected\n");
        }
        ImGui::EndPopup();
    }
}
