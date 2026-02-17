#include "Inspector.hpp"

#include <iostream>
#include <typeindex>

#include "Components/Primitives/Transform.hpp"
#include "Components/Rendering/GizmoControls.hpp"
#include "Components/Constants.hpp"

#include "Core/EditorWindows/Hierarchy.hpp"

#include "imgui.h"
#include "ImGuizmo.h"
#include <glm/gtc/type_ptr.hpp>

#include "Components/Inspectable.hpp"
#include "Components/SceneRoot.hpp"
#include "Components/Rendering/DirectionalLight.hpp"
#include "Components/Rendering/PointLight.hpp"
#include "Systems/SimObjectSystem.hpp"

namespace Inspector {

    template<typename... Component>
    void GetComponents(entt::registry& registry, entt::entity entity, std::vector<std::string> &names, entt::type_list<Component...>) {
        ([&]() {
            if (registry.any_of<Component>(entity)) {
                auto& comp = registry.get<Component>(entity);
                names.push_back(static_cast<std::string>(entt::type_name<Component>::value()));
            }
        }(), ...);
    }

    static float frameTimes[128] = {};
    static int frameIndex = 0;
    void Show(entt::registry &registry, const entt::entity &selectedObject) {
        // Store All Inspectable Names as strings in vector
        std::vector<std::string> component_names;
        GetComponents(registry, selectedObject, component_names, Inspectable::InspectableComponents{});

        const auto &constants = registry.ctx().get<Constants>();
        auto &[operation, mode] = registry.ctx().get<GizmoControls>();

        ImGui::SetNextWindowPos(ImVec2(constants.SCENE_X + constants.SCENE_WIDTH, 0), ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(ImVec2(constants.WINDOW_WIDTH - (constants.SCENE_WIDTH + constants.SCENE_X), constants.WINDOW_HEIGHT), ImGuiCond_Appearing);
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

        for (const auto& name : component_names)
        {
            // BUG: using strings may contain typos
            if (name == "Transform") ShowTransform(registry, selectedObject);
            else if (name == "DirectionalLight") ShowDirectionalLight(registry, selectedObject);
            else if (name == "PointLight") ShowPointLight(registry, selectedObject);
            else if (name == "Camera") ShowCamera(registry, selectedObject);
        }

        ImGui::End();

        // TODO: Should not be here. A new class has to be created, preferrably with global access
        ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_NoCollapse);
        ShowDebugInfo();
        ImGui::End();

    }

    void Hide(entt::registry &registry)
    {
        const auto &constants = registry.ctx().get<Constants>();
        ImGui::SetNextWindowPos(ImVec2(constants.SCENE_X + constants.SCENE_WIDTH, 0), ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(ImVec2(constants.WINDOW_WIDTH - (constants.SCENE_WIDTH + constants.SCENE_X), constants.WINDOW_HEIGHT), ImGuiCond_Appearing);
        ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        ImGui::End();
    }

    void ShowTransform(entt::registry& registry, const entt::entity& selectedObject)
{
    if (selectedObject == entt::null) return;

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        auto &transform = registry.get<Transform>(selectedObject);

        // Name
        char nameBuf[128];
        strncpy(nameBuf, transform.name.c_str(), sizeof(nameBuf));
        if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf))) {
            transform.name = nameBuf;
        }

        bool changed = false;

        // Position
        if (ImGui::DragFloat3("Position", glm::value_ptr(transform.position), 0.05f)) {
            changed = true;
        }

        // Rotation (Euler conversion)
        if (ImGui::DragFloat3("Rotation", glm::value_ptr(transform.eulerAngles), 0.5f)) {
            transform.rotation = glm::quat(glm::radians(transform.eulerAngles));
            changed = true;
        }

        // Scale
        if (ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale), 0.05f)) {
            changed = true;
        }

        // 1. Reset Scale safety
        if (transform.scale.x < 0.001f) transform.scale.x = 0.001f;
        if (transform.scale.y < 0.001f) transform.scale.y = 0.001f;
        if (transform.scale.z < 0.001f) transform.scale.z = 0.001f;

        // 2. Trigger Hierarchy Update if values changed via UI
        if (changed) {
            // Get your SceneRoot or call your UpdateHierarchyMatrices function here
            // to make sure children follow the slider movement instantly.
            const auto root = registry.ctx().get<SceneRoot>().entity;
            SimObject::UpdateHierarchyMatrices(registry, root, glm::mat4(1.0f));
        }

        // Optional: Read-only World Position for debugging
        ImGui::BeginDisabled();
        glm::vec3 worldPos = glm::vec3(transform.model[3]);
        ImGui::DragFloat3("World Position", glm::value_ptr(worldPos));
        ImGui::EndDisabled();
    }
}

    void ShowDirectionalLight(entt::registry& registry, const entt::entity& selectedObject)
    {
        if (ImGui::CollapsingHeader("Directional Light", ImGuiTreeNodeFlags_DefaultOpen) && selectedObject != entt::null) {

            auto &dirLight = registry.get<DirectionalLight>(selectedObject);
            ImGui::ColorEdit3("Ambient", glm::value_ptr(dirLight.ambient));
            ImGui::ColorEdit3("Diffuse", glm::value_ptr(dirLight.diffuse));
            ImGui::ColorEdit3("Specular", glm::value_ptr(dirLight.specular));
        }
    }

    void ShowPointLight(entt::registry& registry, const entt::entity& selectedObject)
    {
        if (ImGui::CollapsingHeader("Point Light", ImGuiTreeNodeFlags_DefaultOpen) && selectedObject != entt::null) {

            auto &pointLight = registry.get<PointLight>(selectedObject);
            ImGui::ColorEdit3("Ambient", glm::value_ptr(pointLight.ambient));
            ImGui::ColorEdit3("Diffuse", glm::value_ptr(pointLight.diffuse));
            ImGui::ColorEdit3("Specular", glm::value_ptr(pointLight.specular));
        }
    }

    void ShowCamera(entt::registry& registry, const entt::entity& selectedObject)
    {
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen) && selectedObject != entt::null) {
            auto &camera = registry.get<Camera>(selectedObject);

            // TODO: These are for debugging
            ImGui::PushItemWidth(90.0);
            ImGui::SliderFloat("Movement Speed", &camera.MovementSpeed, 0.01, 10.0);
            ImGui::SliderFloat("Mouse Sensitivity", &camera.MouseSensitivity, 0.01, 1.0);

            ImGui::SliderFloat("FOV", &camera.Fov, 0.01, 180.0);
            ImGui::SliderFloat("Zoom", &camera.Zoom, 0.01, 10.0);
            ImGui::PopItemWidth();
        }
    }

    void ShowDebugInfo()
    {
        const ImGuiIO& io = ImGui::GetIO();

        // FPS & Timing (always available)
        ImGui::Text("FPS: %.1f (%.1f ms)", io.Framerate, 1000.0f/io.Framerate);

        // Mouse/Keyboard input
        ImGui::Text("Mouse: (%.1f, %.1f)", io.MousePos.x, io.MousePos.y);
        ImGui::Text("Delta: (%.1f, %.1f)", io.MouseDelta.x, io.MouseDelta.y);

        float frameTime = 1000.0f / io.Framerate;
        frameTimes[frameIndex] = frameTime;
        frameIndex = (frameIndex + 1) % 128;

        ImGui::PlotLines("Frame Time (ms)", frameTimes, 128, 0,
                         nullptr, 0.0f, 16.67f,
                         ImVec2(60.0f, 30.0f));

        ImGui::Separator();
    }

}
