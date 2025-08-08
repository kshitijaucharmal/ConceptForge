//
// Created by kshitij on 8/8/25.
//
#include <entt/entt.hpp>
#include <Systems/InputSystem.hpp>
#include <Systems/RayTracer/RayTracer.hpp>
#include <utility>

#include "Core/WindowManager.hpp"
#include "Core/GUISystem.hpp"
#include "Core/EventSystem.hpp"
#include "Core/SSBOManager.hpp"

#include "Components/Camera.hpp"
#include "Components/Constants.hpp"
#include "Components/SSBOHolder.hpp"
#include "Components/Time.hpp"
#include "Components/Physics/BulletPhysics.hpp"
#include "Components/RayTracer/RayTracerSettings.hpp"
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
#include "Systems/Debug.hpp"
#include "Systems/Primitives/GridSystem.hpp"
#include "Systems/Rendering/LightSystem.hpp"


class App {
//Variables
public:
    entt::registry registry;
    Constants constants;

    Window window;
    entt::entity camera;

    GameState *gameState;

    // Default Shaders
    entt::entity debugShader;
    entt::entity gridShader;
    entt::entity litShader;
    entt::entity unlitShader;

    // For Raytracing
    entt::entity rtShader;
    entt::entity rtEntity;

    // Grid
    entt::entity grid;

private:
    // Init Functions
    void InitContext() {
        // Global Values (Context)
        registry.ctx().emplace<Debug::DebugInfo>();
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
        registry.ctx().emplace<RayTracerSettings>();
    }
    void InitRenderSystem() {
        // Initialize the FrameBuffer
        RenderSystem::Init(registry);

        // Init SSBOs
        SSBOManager::AddAndInit(registry, "pointLights", 1);
        SSBOManager::AddAndInit(registry, "dirLights", 2);
        SSBOManager::AddAndInit(registry, "rtSettings", 3);
    }
    void InitCameras() {
        // Camera ----------------------------------------------------------
        // Create camera (Can be many, but one for now)
        camera = CameraSystem::CreateCamera(registry, "Main Camera");

        // Set it as the active camera
        registry.ctx().insert_or_assign<ActiveCamera>({camera});
        // ------------------------------------------------------------------
    }
    void InitShaders() {
        // Shader System ----------------------------------------------------
        // Shaders
        auto &shaderStore = registry.ctx().get<ShaderStore>();

        debugShader = registry.create();
        registry.emplace<Shader>(debugShader, Shader{
            .vertexShaderPath = SHADER_DIR "/point.vert",
            .fragmentShaderPath = SHADER_DIR "/point.frag"
        });
        shaderStore.shaders["DebugShader"] = debugShader;

        gridShader = registry.create();
        registry.emplace<Shader>(gridShader, Shader{
            .vertexShaderPath = SHADER_DIR "/grid.vert",
            .fragmentShaderPath = SHADER_DIR "/grid.frag"
        });
        shaderStore.shaders["GridShader"] = gridShader;

        litShader = registry.create();
        registry.emplace<Shader>(litShader, Shader{
            .vertexShaderPath = constants.SP_LIT_VERT,
            .fragmentShaderPath = constants.SP_LIT_FRAG
        });
        shaderStore.shaders["LitShader"] = litShader;

        // Unlit Shader
        unlitShader = registry.create();
        registry.emplace<Shader>(unlitShader, Shader{
            .vertexShaderPath = constants.SP_UNLIT_VERT,
            .fragmentShaderPath = constants.SP_UNLIT_FRAG
        });
        shaderStore.shaders["UnlitShader"] = unlitShader;

        // RayTracer Shader
        rtShader = registry.create();
        registry.emplace<Shader>(rtShader, Shader{
            .vertexShaderPath = SHADER_DIR "/rtShader.vert",
            .fragmentShaderPath = SHADER_DIR "/rtShader.frag"
        });
        shaderStore.shaders["RayTracingShader"] = rtShader;

        // Init
        MaterialSystem::InitWhiteTexture(registry);
        ShaderSystem::InitShaders(registry);

        rtEntity = registry.create();
        RayTracer::Init(registry, rtEntity);
    }
    void SetupScene() {
        // Grid
        grid = GridSystem::CreateGrid(registry, gridShader, "Grid");

        // Ground (Static)
        {
            auto transform = Transform{
                .name = "Ground",
                .position = glm::vec3(0.0f, 0, 0.0f),
                .rotation = glm::quat(1, 0, 0, 0),
                .scale = glm::vec3(20.0, .01f, 20.0f)
            };
            Primitives::CreateCubeObject(registry, transform, litShader, false);
        }
        // Point Lights
        {
            auto transform = Transform{
                .name = "Point Light",
                .position = glm::vec3(0.0f, 10.0f, 0.0f),
                .rotation = glm::quat(1, 0, 0, 0),
                .scale = glm::vec3(0.3, 0.3, 0.3)
            };
            LightSystem::AddPointLight(registry, transform);
        }
        // Directional Lights
        {
            auto transform = Transform{
                .name = "Directional Light",
                .position = glm::vec3(0.0f, 5.0f, 0.0f),
                .rotation = glm::quat(1, 0, 0, 0),
                .scale = glm::vec3(0.3, 0.3, 0.3)
            };
            LightSystem::AddDirectionalLight(registry, transform);
        }
        // ------------------------------------------------------------------

    }
    void InitDebuggingAndUI() {
        // ImGUI ------------------------------------------------------------
        // Setting up ImGUI
        GUISystem::InitImGUI(registry, window.window);
        Debug::Init(registry);
        // ------------------------------------------------------------------
    }

public:
    App(const int width, const int height, const std::string &windowName) :
        constants(registry.ctx().emplace<Constants>()),
        window(registry, width, height, windowName)
    {
        // Window window(registry, width, height, windowName);
        InitContext();
        InitRenderSystem();

        // Physics
        BulletPhysicsSystem::Init(registry);
        // ------------------------------------------------------------------

        InitCameras();
        InitShaders();
        InitDebuggingAndUI();

        SetupScene();

        // Call Awake
        auto &awakeQueue = registry.ctx().get<EventSystem::AwakeQueue>();
        for (auto &fn : awakeQueue.functions) fn();
    }
    bool ShouldQuit() {
        return glfwWindowShouldClose(window.window);
    }
    void SimulatePhysics() {
        BulletPhysicsSystem::StepSimulation(registry, registry.ctx().get<Time>().deltaTime);
        BulletPhysicsSystem::SyncTransforms(registry);
    }
    void RT_Rendering() {
        RayTracer::Render(registry, rtEntity);
    }
    void Normal_Rendering() {
        // Normal Rendering
        // Grid
        GridSystem::Render(registry, grid, registry.get<Shader>(gridShader));
        // Render every object
        RenderSystem::Render(registry);
        // Debug::DrawPoint(registry, glm::vec3(0, 3, 0), 20);
    }
    void UI_Rendering() {
        // UI here ------------------------------------------------------
        GUISystem::NewFrame();

        // Editor Windows
        Hierarchy::Show(registry);
        Inspector::Show(registry);

        // Custom Windows
        auto &imguiQueue = registry.ctx().get<GUISystem::ImGuiDrawQueue>();
        for (auto &fn : imguiQueue) fn();

        ImGui::Begin("Settings");
        ImGui::Checkbox("RayTracing", &gameState->rayTracing);
        ImGui::Checkbox("Playing", &gameState->isPlaying);
        ImGui::End();

        // Before drawing anything, clear screen
        Window::ScreenClearFlags(constants.CLEAR_COLOR);
        RenderSystem::ShowSceneTexture(registry, window.window);
        GUISystem::RenderFrame();

        // Clear queue for next frame
        imguiQueue.clear();
        // --------------------------------------------------------------
    }
    void CleanUp() {
        // Shutdown Physics
        BulletPhysicsSystem::Shutdown(registry);

        // Destroy GUI System
        GUISystem::Destroy();
    }

    void MainLoop() {
        gameState = &registry.ctx().get<GameState>();
        // Main Loop --------------------------------------------------------
        while(!ShouldQuit()){
            // DeltaTime
            CalculateDeltaTime(registry);

            // Update here --------------------------------------------------

            if (gameState->isPlaying) {
                SimulatePhysics();
            }

            // Input System
            InputSystem::ProcessInput(registry, window.window);

            // Call every System in Update/LateUpdate
            // Update
            auto &updateQueue = registry.ctx().get<EventSystem::UpdateQueue>();
            for (auto &fn : updateQueue.functions) fn();

            // Camera Projection
            CameraSystem::CalculateProjection(registry);
            CameraSystem::SetView(registry);

            // Call Late Update
            auto &lateUpdateQueue = registry.ctx().get<EventSystem::LateUpdateQueue>();
            for (auto &fn : lateUpdateQueue.functions) fn();
            // --------------------------------------------------------------

            // Rendering --------------------------------------------------
            // Anything inside framebuffer draws to the Scene window
            RenderSystem::BindFramebuffer(registry);
            {
                // Program Background
                Window::ScreenClearFlags(constants.BACKGROUND_COLOR);

                // Ray Tracing
                if (gameState->rayTracing){
                    RT_Rendering();
                }
                else {
                    Normal_Rendering();
                }
            }
            RenderSystem::UnbindFramebuffer();
            // --------------------------------------------------------------

            // Push To Draw Queue -------------------------------------------
            // --------------------------------------------------------------

            UI_Rendering();

            // check and call events
            glfwPollEvents();
            glfwSwapBuffers(window.window);

        }
        // ------------------------------------------------------------------
        CleanUp();
    }
};
