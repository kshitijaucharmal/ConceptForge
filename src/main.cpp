#include <entt/entt.hpp>
#include <Systems/InputSystem.hpp>
#include <Systems/RayTracer/RayTracer.hpp>

#include "Core/WindowManager.hpp"
#include "Core/GUISystem.hpp"
#include "Core/EventSystem.hpp"
#include "Core/SSBOManager.hpp"

#include "Components/Camera.hpp"
#include "Components/Constants.hpp"
#include "Components/SSBOHolder.hpp"
#include "Components/Time.hpp"
#include "Components/Physics/BulletPhysics.hpp"
#include "Components/Rendering/FrameBuffer.hpp"
#include "Components/Rendering/GizmoControls.hpp"
#include "Core/GameState.hpp"

#include "Systems/CameraSystem.hpp"
#include "Systems/TimeSystem.hpp"
#include "Systems/Rendering/MaterialSystem.hpp"
#include "Systems/Rendering/ShaderSystem.hpp"
#include "Systems/Rendering/RenderSystem.hpp"
#include "Systems/Primitives/PrimitivesSystem.hpp"
#include "Systems/Primitives/GizmoSystem.hpp"

#include "Core/EditorWindows/Inspector.hpp"
#include "Core/EditorWindows/Hierarchy.hpp"
#include "Core/Physics/PhysicsSystem.hpp"
#include "Systems/Primitives/GridSystem.hpp"
#include "Systems/Rendering/LightSystem.hpp"

int main(){
    entt::registry registry;

    // Setup Global Constants (Might be changed)
    auto constants = registry.ctx().emplace<Constants>();

    // Global Values (Context)
    registry.ctx().emplace<GameState>();
    registry.ctx().emplace<Hierarchy::Hierarchy>();
    registry.ctx().emplace<Time>();
    registry.ctx().emplace<ActiveCamera>();
    registry.ctx().emplace<EventSystem::AwakeQueue>();
    registry.ctx().emplace<EventSystem::UpdateQueue>();
    registry.ctx().emplace<EventSystem::LateUpdateQueue>();
    registry.ctx().emplace<MaterialSystem::WhiteTexture>(0);
    registry.ctx().emplace<GUISystem::ImGuiDrawQueue>();
    registry.ctx().emplace<FrameBuffer>();
    registry.ctx().emplace<ShaderStore>();
    registry.ctx().emplace<SSBOHolder>();
    registry.ctx().emplace<LightSystem::PointLightsHandle>();
    registry.ctx().emplace<LightSystem::DirectionalLightsHandle>();
    registry.ctx().emplace<GizmoControls>();
    registry.ctx().emplace<BulletPhysics>();

    // Window -----------------------------------------------------------
    // initialize window (and OpenGL)
    Window window(registry, 1600, 900, "ConceptForge");

    // Initialize the FrameBuffer
    RenderSystem::Init(registry);
    // Init SSBOs
    SSBOManager::AddAndInit(registry, "pointLights", 1);
    SSBOManager::AddAndInit(registry, "dirLights", 2);

    // Physics
    BulletPhysicsSystem::Init(registry);

    // ------------------------------------------------------------------

    // Camera ----------------------------------------------------------
    // Create camera (Can be many, but one for now)
    auto camera = CameraSystem::CreateCamera(registry, "Main Camera");

    // Set it as the active camera
    registry.ctx().insert_or_assign<ActiveCamera>({camera});
    // ------------------------------------------------------------------


    // Shader System ----------------------------------------------------
    // Shaders
    auto &shaderStore = registry.ctx().get<ShaderStore>();
    entt::entity gridShader = registry.create();
    registry.emplace<Shader>(gridShader, Shader{
        .vertexShaderPath = SHADER_DIR "/grid.vert",
        .fragmentShaderPath = SHADER_DIR "/grid.frag"
    });
    shaderStore.shaders["GridShader"] = gridShader;

    entt::entity litShader = registry.create();
    registry.emplace<Shader>(litShader, Shader{
        .vertexShaderPath = constants.SP_LIT_VERT,
        .fragmentShaderPath = constants.SP_LIT_FRAG
    });
    shaderStore.shaders["LitShader"] = litShader;

    // Unlit Shader
    entt::entity unlitShader = registry.create();
    registry.emplace<Shader>(unlitShader, Shader{
        .vertexShaderPath = constants.SP_UNLIT_VERT,
        .fragmentShaderPath = constants.SP_UNLIT_FRAG
    });
    shaderStore.shaders["UnlitShader"] = unlitShader;

    // RayTracer Shader
    entt::entity rtShader = registry.create();
    registry.emplace<Shader>(rtShader, Shader{
        .vertexShaderPath = SHADER_DIR "/rtShader.vert",
        .fragmentShaderPath = SHADER_DIR "/rtShader.frag"
    });
    shaderStore.shaders["RayTracingShader"] = rtShader;

    // Init
    MaterialSystem::InitWhiteTexture(registry);
    ShaderSystem::InitShaders(registry);

    auto rtEntity = registry.create();
    RayTracer::Init(registry, rtEntity);

    // Grid
    const auto grid = GridSystem::CreateGrid(registry, gridShader, "Grid");

    // Ground (Static)
    {
        auto transform = Transform{
            .name = "Ground",
            .position = glm::vec3(0.0f, 0, 0.0f),
            .rotation = glm::quat(1, 0, 0, 0),
            .scale = glm::vec3(20.0, .01f, 20.0f)
        };
        entt::entity ground = Primitives::CreateCubeObject(registry, transform, litShader, false);
    }
    // ------------------------------------------------------------------

    // ImGUI ------------------------------------------------------------
    // Setting up ImGUI
    GUISystem::InitImGUI(registry, window.window);
    // ------------------------------------------------------------------

    // Awake
    auto &awakeQueue = registry.ctx().get<EventSystem::AwakeQueue>();
    for (auto &fn : awakeQueue.functions) fn();


    auto &gameState = registry.ctx().get<GameState>();
    // Main Loop --------------------------------------------------------
    while(!glfwWindowShouldClose(window.window)) {
        // Update here --------------------------------------------------
        CalculateDeltaTime(registry);
        if (gameState.isPlaying) {
            BulletPhysicsSystem::StepSimulation(registry, registry.ctx().get<Time>().deltaTime);
            BulletPhysicsSystem::SyncTransforms(registry);
        }
        InputSystem::ProcessInput(registry, window.window);
        CameraSystem::CalculateProjection(registry);
        CameraSystem::SetView(registry);

        // Call every System in Update/LateUpdate
        // Update
        auto &updateQueue = registry.ctx().get<EventSystem::UpdateQueue>();
        for (auto &fn : updateQueue.functions) fn();

        // Late Update
        auto &lateUpdateQueue = registry.ctx().get<EventSystem::LateUpdateQueue>();
        for (auto &fn : lateUpdateQueue.functions) fn();
        // --------------------------------------------------------------

        // Render here --------------------------------------------------
        // Anything inside framebuffer draws to the Scene window
        RenderSystem::BindFramebuffer(registry);
        // Program Background
        Window::ScreenClearFlags(constants.BACKGROUND_COLOR);

        // Ray Tracing
        if (gameState.rayTracing){
            RayTracer::Render(registry, rtEntity);
        }
        else {
            // Normal Rendering
            // Grid
            GridSystem::Render(registry, grid, registry.get<Shader>(gridShader));
            // Render every object
            RenderSystem::Render(registry);

        }

        RenderSystem::UnbindFramebuffer();
        // --------------------------------------------------------------

        // Push To Draw Queue -------------------------------------------
        // --------------------------------------------------------------

        // UI here ------------------------------------------------------
        GUISystem::NewFrame();

        // Editor Windows
        Hierarchy::Show(registry);
        Inspector::Show(registry);

        // Custom Windows
        auto &imguiQueue = registry.ctx().get<GUISystem::ImGuiDrawQueue>();
        for (auto &fn : imguiQueue) fn();

        ImGui::Begin("Settings");
        ImGui::Checkbox("RayTracing", &gameState.rayTracing);
        ImGui::End();

        // Before drawing anything, clear screen
        Window::ScreenClearFlags(constants.CLEAR_COLOR);
        RenderSystem::ShowSceneTexture(registry, window.window);
        GUISystem::RenderFrame();

        // Clear queue for next frame
        imguiQueue.clear();
        // --------------------------------------------------------------

        // check and call events
        glfwPollEvents();
        glfwSwapBuffers(window.window);

    }
    // ------------------------------------------------------------------
    BulletPhysicsSystem::Shutdown(registry);

    GUISystem::Destroy();
}
