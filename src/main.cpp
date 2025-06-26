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
#include "Systems/Rendering/RenderSystem.hpp"
#include "Systems/Primitives/CubeSystem.hpp"
#include "Systems/SimObjectSystem.hpp"

int main(){
    entt::registry registry;

    // Setup Global Constants (Might be changed)
    auto constants = registry.ctx().emplace<Constants>();

    // Global Values (Context)
    registry.ctx().emplace<Time>();
    registry.ctx().emplace<ActiveCamera>();
    registry.ctx().emplace<EventSystem::AwakeQueue>();
    registry.ctx().emplace<EventSystem::UpdateQueue>();
    registry.ctx().emplace<EventSystem::LateUpdateQueue>();
    registry.ctx().emplace<MaterialSystem::WhiteTexture>();
    registry.ctx().emplace<GUISystem::ImGuiDrawQueue>();

    // Window -----------------------------------------------------------
    // initialize window (and OpenGL)
    Window window(registry, 1600, 900, "ConceptForge");
    // ------------------------------------------------------------------

    // Camera ----------------------------------------------------------
    // Create camera (Can be many, but one for now)
    auto camera = registry.create();
    registry.emplace<Camera>(camera, Camera{
        .Position = glm::vec3(7.3589f, 7.3444f, 6.9258f),
        .Yaw = -132.4,
        .Pitch = -28.2f,
        .MovementSpeed = 3.0f,
        .Fov = 55.0f
        // More settings can be changed
    });
    glm::vec3 origin = glm::vec3(0.0);
    glm::vec3 front = glm::vec3(0.0, 0.0, -1.0);
    LookAt(registry.get<Camera>(camera), origin + front);

    // Set it as the active camera
    registry.ctx().insert_or_assign<ActiveCamera>({camera});

    // Only initialize _this_ camera
    InitCamera(registry, camera);
    // ------------------------------------------------------------------

    // Shader System ----------------------------------------------------
    // Shaders
    entt::entity litShader = registry.create();
    registry.emplace<Shader>(litShader, Shader{
        .vertexShaderPath = constants.SP_LIT_VERT,
        .fragmentShaderPath = constants.SP_LIT_FRAG
    });

    entt::entity unlitShader = registry.create();
    registry.emplace<Shader>(unlitShader, Shader{
        .vertexShaderPath = constants.SP_UNLIT_VERT,
        .fragmentShaderPath = constants.SP_UNLIT_FRAG
    });
    ShaderSystem::InitShaders(registry);

    // Cube Object
    auto transform = Transform{
        .name = "Cube",
        .position = glm::vec3(0.0, 2.0, 0.0)
    };
    auto transform2 = Transform{
        .name = "Cube2",
        .position = glm::vec3(0.0, -2.0, 0.0)
    };
    CubeSystem::CreateCubeObject(registry, transform2, litShader);
    CubeSystem::CreateCubeObject(registry, transform, unlitShader);
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
        RenderSystem::Render(registry);
        // --------------------------------------------------------------

        // Push To Draw Queue -------------------------------------------

        // --------------------------------------------------------------

        // UI here ------------------------------------------------------
        GUISystem::NewFrame();

        // Show Every window in the Draw Queue
        auto &imguiQueue = registry.ctx().get<GUISystem::ImGuiDrawQueue>();
        for (auto &fn : imguiQueue) fn();

        ImGui::ShowMetricsWindow();

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
