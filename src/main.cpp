#include <entt/entt.hpp>
#include <iostream>

#include "Core/WindowManager.hpp"
#include "Core/GUISystem.hpp"
#include "Core/EventSystem.hpp"

#include "Components/Camera.hpp"
#include "Components/Constants.hpp"
#include "Components/Time.hpp"

#include "Systems/CameraSystem.hpp"
#include "Systems/TimeSystem.hpp"
#include "Systems/Rendering/MaterialSystem.hpp"
#include "Systems/Rendering/ShaderSystem.hpp"

int main(){
    entt::registry registry;

    // Setup Global Constants (Might be changed)
    auto constants = registry.ctx().emplace<Constants>();

    // Global Values required (like Time)
    registry.ctx().emplace<Time>();
    registry.ctx().emplace<EventSystem::AwakeQueue>();
    registry.ctx().emplace<EventSystem::UpdateQueue>();
    registry.ctx().emplace<EventSystem::LateUpdateQueue>();
    registry.ctx().emplace<GUISystem::ImGuiDrawQueue>();

    // Shaders
    entt::entity basicShader = registry.create();
    registry.emplace<Shader>(basicShader, Shader{
        .vertexShaderPath = constants.SP_LIT_VERT,
        .fragmentShaderPath = constants.SP_LIT_FRAG
    });

    entt::entity mat1 = registry.create();
    registry.emplace<Material>(mat1, basicShader);

    ShaderSystem::InitShaders(registry);
    // MaterialSystem::InitMaterials(registry);

    return 0;

    // Camera ----------------------------------------------------------
    // Create camera (Can be many, but one for now)
    auto camera = registry.create();
    registry.emplace<Camera>(camera, Camera{
        .Position = glm::vec3(7.3589f, 7.3444f, 6.9258f),
        .Yaw = -132.4,
        .Pitch = -28.2f,
        .MovementSpeed = 3.0f,
        .Fov = 75.0f
        // More settings can be changed
    });

    // Only initialize _this_ camera
    InitCamera(registry, camera);
    // ------------------------------------------------------------------

    // Window -----------------------------------------------------------
    // initialize window
    Window window(registry, 1600, 900, "ConceptForge");
    // ------------------------------------------------------------------

    // ImGUI ------------------------------------------------------------
    // Setting up ImGUI
    GUISystem::InitImGUI(registry, window.window);
    // ------------------------------------------------------------------

    // Awake
    auto &awakeQueue = registry.ctx().get<EventSystem::AwakeQueue>();
    for (auto &fn : awakeQueue.functions) fn();

    // Main Loop --------------------------------------------------------
    while(!glfwWindowShouldClose(window.window)){
        // Update here --------------------------------------------------
        CalculateDeltaTime(registry);
        CalculateProjection(registry);

        // Call every System in Update/LateUpdate
        // Update
        auto &updateQueue = registry.ctx().get<EventSystem::UpdateQueue>();
        for (auto &fn : updateQueue.functions) fn();

        // Late Update
        auto &lateUpdateQueue = registry.ctx().get<EventSystem::LateUpdateQueue>();
        for (auto &fn : lateUpdateQueue.functions) fn();
        // --------------------------------------------------------------

        // Before drawing anything, clear screen
        window.ScreenClearFlags(registry);

        // Render here --------------------------------------------------

        // --------------------------------------------------------------

        // UI here ------------------------------------------------------
        GUISystem::NewFrame();

        // Show Every window in the Draw Queue
        auto &imguiQueue = registry.ctx().get<GUISystem::ImGuiDrawQueue>();
        for (auto &fn : imguiQueue) fn();

        GUISystem::RenderFrame();

        // Clear queue for next frame
        imguiQueue.clear();
        // --------------------------------------------------------------

        // check and call events
        glfwPollEvents();
        glfwSwapBuffers(window.window);

    }
    // ------------------------------------------------------------------
    GUISystem::Destroy();
}
