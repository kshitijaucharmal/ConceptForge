#include "Hierarchy.hpp"
#include "Components/Constants.hpp"
#include "Components/Primitives/Transform.hpp"

#include <iostream>
#include <Components/Rendering/Shader.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/SimObjectSystem.hpp>
#include <Systems/Primitives/PrimitivesSystem.hpp>
#include <Systems/Rendering/LightSystem.hpp>

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

        // Size and position
        const int width = c.WINDOW_WIDTH - (c.SCENE_WIDTH + c.SCENE_X);
        const int height = c.SCENE_HEIGHT;
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Appearing);
        ImGui::Begin("Hierarchy");

        // Center screen if scrolling with arrow keys
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

        // Draw all Transforms
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth;
        for (int i = 0; i < entities.size(); ++i) {

            if (i == selectedID)
                flags |= ImGuiTreeNodeFlags_Selected;

            ImGui::PushID(i);

            if (auto &transform = registry.get<Transform>(entities[i]);
                // Space for padding
                ImGui::Selectable( (" " + transform.name).c_str(), i == selectedID, ImGuiSelectableFlags_SpanAllColumns)) {
                selectedID = i;
                }
            // Right-click on item
            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                selectedID = i;
                std::cout << selectedID << std::endl;
                ImGui::OpenPopup("ItemMenu");
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
        }

        // Set selected entitiy
        if(selectedID != -1){
            selectedEntity = entities[selectedID];
        }
        PopupMenus(registry, selectedID);
        ImGui::End();
    }

    void PopupMenus(entt::registry &registry, int selectedID) {
        // Right-click on empty space
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup("EmptySpaceMenu");
        }
        if (ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup("ItemMenu");
        }

        // --------- Item Context Menu ---------
        if (ImGui::BeginPopup("ItemMenu")) {
            if (ImGui::MenuItem("Rename")) {
                // Handle Rename
            }
            if (ImGui::MenuItem("Copy")) {
                // Handle Copy
            }
            if (ImGui::MenuItem("Delete")) {
                // Handle Delete for item
            }

            ImGui::EndPopup();
        }
        // --------- Empty Space Context Menu ---------
        if (ImGui::BeginPopup("EmptySpaceMenu")) {
            if (ImGui::BeginMenu("Create")) {
                if (ImGui::BeginMenu("Primitives")) {
                    if (ImGui::MenuItem("Empty")) {
                        // Handle Empty creation
                        SimObject::Create(registry, "Empty");
                    }
                    if (ImGui::MenuItem("Cube")) {
                        // Handle Cube creation
                        const auto transform = Transform { .name = "Cube", };
                        auto &shaders = registry.ctx().get<ShaderStore>().shaders;
                        Primitives::Create(registry, Primitives::PrimitiveType::CUBE, transform, shaders["LitShader"]);
                    }
                    if (ImGui::MenuItem("UV Sphere")) {
                        // Handle UV Sphere creation
                        const auto transform = Transform { .name = "UV Sphere", };
                        auto &shaders = registry.ctx().get<ShaderStore>().shaders;
                        Primitives::Create(registry, Primitives::PrimitiveType::UV_SPHERE, transform, shaders["LitShader"]);
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Lights")) {
                    if (ImGui::MenuItem("Directional Light")) {
                        // Handle Light creation
                        LightSystem::AddDirectionalLight(registry,
                            Transform{
                                .name = "Directional Light",
                                .position = glm::vec3(0.0, 10.0, 3.0),
                                .rotation = glm::quat(glm::radians(glm::vec3(180.0f, -30.0f, -60.0f))),
                                .scale = glm::vec3(0.4)
                            });
                    }
                    if (ImGui::MenuItem("Point Light")) {
                        // Handle Light creation
                        LightSystem::AddPointLight(registry,
                            Transform{
                                .name = "Point Light",
                                .position = glm::vec3(0.0, 4.0, 3.0),
                                .scale = glm::vec3(0.4)
                            });
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Camera")) {
                    // Handle Empty creation
                    CameraSystem::CreateCamera(registry);
                }
                ImGui::EndMenu();
            }

            ImGui::EndPopup();
        }

    }

}
