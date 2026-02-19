#include "Hierarchy.hpp"

#include <format>

#include "Components/Constants.hpp"
#include "Components/Primitives/Transform.hpp"

#include <iostream>
#include <Components/Rendering/Shader.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/SimObjectSystem.hpp>
#include <Systems/Primitives/PrimitivesSystem.hpp>
#include <Systems/Rendering/LightSystem.hpp>

#include "Components/SceneRoot.hpp"
#include "Core/EventSystem.hpp"

namespace Hierarchy {
    void DrawEntityNode(entt::registry& registry, entt::entity entity, entt::entity& selectedEntity) {
        auto& transform = registry.get<Transform>(entity);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
                                 | ImGuiTreeNodeFlags_OpenOnDoubleClick
                                 | ImGuiTreeNodeFlags_SpanAvailWidth
                                 | ImGuiTreeNodeFlags_DefaultOpen
                                 | ImGuiTreeNodeFlags_SpanFullWidth;

        if (entity == selectedEntity) flags |= ImGuiTreeNodeFlags_Selected;
        if (transform.first_child == entt::null) flags |= ImGuiTreeNodeFlags_Leaf;

        const bool opened = ImGui::TreeNodeEx((void*)(uintptr_t)entity, flags, "%s", transform.name.c_str());

        // --- Drag and Drop Source ---
        if (ImGui::BeginDragDropSource()) {
            // We pass the entity handle as the payload
            ImGui::SetDragDropPayload("HIERARCHY_NODE", &entity, sizeof(entt::entity));

            // Display what we are dragging
            ImGui::Text("Moving %s", transform.name.c_str());
            ImGui::EndDragDropSource();
        }

        // --- Drag and Drop Target ---
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_NODE")) {
                entt::entity draggedEntity = *(const entt::entity*)payload->Data;

                // Safety: Don't parent to self, and don't parent to an existing child
                // (Reparent usually handles this, but good to check)
                if (draggedEntity != entity) {
                    Transform::Reparent(registry, entity, draggedEntity);

                    // Important: Update matrices immediately so the visual doesn't pop
                    auto root = registry.ctx().get<SceneRoot>().entity;
                    SimObject::UpdateHierarchyMatrices(registry, root, glm::mat4(1.0f));
                }
            }
            ImGui::EndDragDropTarget();
        }

        // Selection logic
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            selectedEntity = entity;
        }

        // Highlight
        if (entity == selectedEntity) {
            const ImVec2 min = ImGui::GetItemRectMin();
            const ImVec2 max = ImGui::GetItemRectMax();
            ImGui::GetWindowDrawList()->AddRect(min, max, IM_COL32(0, 215, 255, 255), 4.0f, 0, 2.0f);
        }

        if (opened) {
            entt::entity currentChild = transform.first_child;
            while (currentChild != entt::null) {
                DrawEntityNode(registry, currentChild, selectedEntity);
                currentChild = registry.get<Transform>(currentChild).next_sibling;
            }
            ImGui::TreePop();
        }
    }

    void Show(entt::registry &registry) {
        auto &[entities, selectedID, selectedEntity] = registry.ctx().get<Hierarchy>();
        const auto &c = registry.ctx().get<Constants>();

        const int width = c.WINDOW_WIDTH - (c.SCENE_WIDTH + c.SCENE_X);
        const int height = c.SCENE_HEIGHT;
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Appearing);

        ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 16.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        // Get the root from context
        auto root = registry.ctx().get<SceneRoot>().entity;

        if (registry.valid(root)) {
            DrawEntityNode(registry, root, selectedEntity);
        }

        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_NODE")) {
                entt::entity draggedEntity = *(const entt::entity*)payload->Data;

                if (draggedEntity != root) {
                    Transform::Reparent(registry, root, draggedEntity);
                    SimObject::UpdateHierarchyMatrices(registry, root, glm::mat4(1.0f));
                }
            }
            ImGui::EndDragDropTarget();
        }

        if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(0)) {
            selectedEntity = entt::null;
        }

        PopupMenus(registry, selectedEntity);

        ImGui::End();
        ImGui::PopStyleVar(2);
    }

    void PopupMenus(entt::registry &registry, entt::entity selected_entity) {
        // Right-click on empty space
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup("EmptySpaceMenu");
        }
        if (ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup("ItemMenu");
        }

        // --------- Item Context Menu ---------
        if (ImGui::BeginPopup("ItemMenu")) {
            if (ImGui::MenuItem("Duplicate")) {
                // Handle Duplicate
            }
            if (ImGui::MenuItem("Delete")) {
                registry.ctx().get<EventSystem::DeleteQueue>().entities.insert(selected_entity);
                selected_entity = entt::null;
            }

            ImGui::EndPopup();
        }
        // --------- Empty Space Context Menu ---------
        if (ImGui::BeginPopup("EmptySpaceMenu")) {
            entt::entity entity = {};
            if (ImGui::BeginMenu("Create")) {
                if (ImGui::BeginMenu("Primitives")) {
                    if (ImGui::MenuItem("Empty")) {
                        // Handle Empty creation
                        entity = SimObject::Create(registry, "Empty");
                    }
                    if (ImGui::MenuItem("Cube")) {
                        // Handle Cube creation
                        const auto transform = Transform {
                            .name = "Cube",
                            .parent = registry.ctx().get<SceneRoot>().entity,
                        };
                        auto &shaders = registry.ctx().get<ShaderStore>().shaders;
                        entity = Primitives::Create(registry, Primitives::PrimitiveType::CUBE, transform, shaders["LitShader"]);
                    }
                    if (ImGui::MenuItem("UV Sphere")) {
                        // Handle UV Sphere creation
                        const auto transform = Transform {
                            .name = "UV Sphere",
                        };
                        auto &shaders = registry.ctx().get<ShaderStore>().shaders;
                        entity = Primitives::Create(registry, Primitives::PrimitiveType::UV_SPHERE, transform, shaders["LitShader"]);
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Lights")) {
                    if (ImGui::MenuItem("Directional Light")) {
                        // Handle Light creation
                        entity = LightSystem::AddDirectionalLight(registry,
                            Transform{
                                .name = "Directional Light",
                                .position = glm::vec3(0.0, 10.0, 3.0),
                                .rotation = glm::quat(glm::radians(glm::vec3(180.0f, -30.0f, -60.0f))),
                                .scale = glm::vec3(0.4),
                            },
                            DirectionalLight());
                    }
                    if (ImGui::MenuItem("Point Light")) {
                        // Handle Light creation
                        entity = LightSystem::AddPointLight(registry,
                            Transform{
                                .name = "Point Light",
                                .position = glm::vec3(0.0, 4.0, 3.0),
                                .scale = glm::vec3(0.4),
                            },
                            PointLight());
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Camera")) {
                    entity = CameraSystem::CreateCamera(registry);
                }
                ImGui::EndMenu();
            }

            auto &hierarchy = registry.ctx().get<Hierarchy>();
            hierarchy.selectedEntity = entity;

            ImGui::EndPopup();
        }

    }

}
