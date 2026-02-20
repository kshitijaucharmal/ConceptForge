//
// Created by kshitij on 8/8/25.
//

#include <btBulletDynamicsCommon.h> // bullet first

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
#include "Core/EditorWindows/PythonEditor.hpp"
#include "Core/EditorWindows/Hierarchy.hpp"
#include "Core/Physics/PhysicsSystem.hpp"
#include "Systems/Debug.hpp"
#include "Systems/Primitives/GridSystem.hpp"
#include "Systems/Rendering/LightSystem.hpp"

#include <Systems/Rendering/Model.hpp>

#include "Components/Fonts.hpp"
#include "Components/GPUTimer.hpp"
#include "Components/SceneRoot.hpp"
#include "Components/Rendering/LightPassFrameBuffer.hpp"
#include "Core/EditorWindows/PythonEditor.hpp"
#include "Systems/LightDepthPassSystem.hpp"
#include "Systems/SceneRootSystem.hpp"
#include "Systems/SimObjectSystem.hpp"

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
    entt::entity borderShader;
    entt::entity toonShader;
    // Minimal Shader to draw position data into depth buffer
    entt::entity shadowShader;

    // For Raytracing
    entt::entity rtShader;
    entt::entity rtEntity;

    // Grid
    entt::entity grid;
    ModelSystem::Model *myModel;

private:
    // Init Functions
    void InitContext() {
        // Global Values (Context)
        registry.ctx().emplace<Debug::DebugInfo>();
        registry.ctx().emplace<GameState>();
        registry.ctx().emplace<Fonts>();
        registry.ctx().emplace<Time>();
        registry.ctx().emplace<ActiveCamera>();
        registry.ctx().emplace<SceneRoot>();
        registry.ctx().emplace<EventSystem::AwakeQueue>();
        registry.ctx().emplace<EventSystem::UpdateQueue>();
        registry.ctx().emplace<EventSystem::LateUpdateQueue>();
        registry.ctx().emplace<EventSystem::DeleteQueue>();
        registry.ctx().emplace<MaterialSystem::FallbackTexture>(0);
        registry.ctx().emplace<GUISystem::ImGuiDrawQueue>();
        registry.ctx().emplace<Hierarchy::Hierarchy>();
        registry.ctx().emplace<FrameBuffer>();
        registry.ctx().emplace<LightPassFrameBuffer>();
        registry.ctx().emplace<ShaderStore>();
        registry.ctx().emplace<SSBOHolder>();
        registry.ctx().emplace<GizmoControls>();
        registry.ctx().emplace<BulletPhysics>();
        registry.ctx().emplace<RayTracerSettings>();
    }
    void InitRenderSystem() {
        // Initialize the FrameBuffers
        LightDepthPassSystem::Init(registry);
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

        // Toon (Cel) Shader
        toonShader = registry.create();
        registry.emplace<Shader>(toonShader, Shader{
            .vertexShaderPath = SHADER_DIR "/toon.vert",
            .fragmentShaderPath = SHADER_DIR "/toon.frag"
        });
        shaderStore.shaders["ToonShader"] = toonShader;

        // Shadow Shader
        shadowShader = registry.create();
        registry.emplace<Shader>(shadowShader, Shader{
            .vertexShaderPath = SHADER_DIR "/shadow.vert",
            .fragmentShaderPath = SHADER_DIR "/shadow.frag"
        });
        shaderStore.shaders["ShadowShader"] = shadowShader;

        // Border Shader
        borderShader = registry.create();
        registry.emplace<Shader>(borderShader, Shader{
            .vertexShaderPath = SHADER_DIR "/border.vert",
            .fragmentShaderPath = SHADER_DIR "/border.frag",
        });
        shaderStore.shaders["BorderShader"] = borderShader;

        // RayTracer Shader
        rtShader = registry.create();
        registry.emplace<Shader>(rtShader, Shader{
            .vertexShaderPath = SHADER_DIR "/rtShader.vert",
            .fragmentShaderPath = SHADER_DIR "/rtShader.frag"
        });
        shaderStore.shaders["RayTracingShader"] = rtShader;

        // Init
        MaterialSystem::InitFallbackTexture(registry);
        ShaderSystem::InitShaders(registry);

        rtEntity = registry.create();
        RayTracer::Init(registry, rtEntity);
    }
    void SetupScene() {
        auto &scene_root = registry.ctx().get<SceneRoot>().entity;
        // Grid
        grid = GridSystem::CreateGrid(registry, gridShader, "Grid");

        {
            auto transform = Transform{
                .name = "Ground",
                .position = glm::vec3(0.0f, -0.5f, 0.0f),
                .rotation = glm::quat(1, 0, 0, 0),
                .scale = glm::vec3(20.0, 1.0f, 20.0f),
            };
            Primitives::CreateCubeObject(registry, transform, litShader, false);
        }
        {
            auto transform = Transform{
                .name = "Cube",
                .position = glm::vec3(0.0f, 2.0f, 0.0f),
                .rotation = glm::quat(1, 0, 0, 0),
                .scale = glm::vec3(1.0, 1.0f, 1.0f),
            };
            auto cube = Primitives::CreateCubeObject(registry, transform, litShader, true);

            auto transform2 = Transform{
                .name = "Cube 1",
                .position = glm::vec3(0.0f, 6.0f, 0.0f),
                .rotation = glm::quat(1, 0, 0, 0),
                .scale = glm::vec3(1.0),
            };
            auto cube2= Primitives::CreateCubeObject(registry, transform2, litShader, true);

            auto transform3 = Transform{
                .name = "Cube 2",
                .position = glm::vec3(0.0f, 10.0f, 0.0f),
                .rotation = glm::quat(1, 0, 0, 0),
                .scale = glm::vec3(1.0),
            };
            auto cube3 = Primitives::CreateCubeObject(registry, transform3, litShader, true);
        }

        // Sphere
        {
            auto transform = Transform{
                .name = "Sphere",
                .position = glm::vec3(-2.0f, 2.0f, 0.0f),
                .rotation = glm::quat(1, 0, 0, 0),
                .scale = glm::vec3(2.0f),
            };
            Primitives::CreateUVSphereObject(registry, transform, litShader, true);
        }

        // Directional Lights
        {
            auto transform = Transform{
                .name = "Directional Light",
                .position = glm::vec3(0.0f, 10.0f, 0.0f),
                .rotation = glm::quat(0.6484594, 0.2819582, -0.6484594, -0.2819582),
                .scale = glm::vec3(0.3, 0.3, 0.3),
            };
            auto light = DirectionalLight{
                .direction = glm::eulerAngles(transform.rotation),
                .ambient = glm::vec3(0.2),
                .diffuse = glm::vec3(1.0),
                .specular = glm::vec3(1.0),
            };
            LightSystem::AddDirectionalLight(registry, transform, light);
        }

        // 3d model
        {
            auto transform = Transform{
                .name = "Backpack",
                .position = glm::vec3(2.5f, 2.5f, 0.0f),
                .scale = glm::vec3(1.0f),
            };
            myModel = new ModelSystem::Model(registry, litShader, MODELS_PATH "/backpack/backpack.obj", transform, false, true);
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
        InitContext();
        InitRenderSystem();

        // Initialize the Scene Root (Every entity with a transform derives from this)
        SceneRootSystem::InitRoot(registry);

        // Physics
        BulletPhysicsSystem::Init(registry);
        // ------------------------------------------------------------------

        InitCameras();
        InitShaders();
        InitDebuggingAndUI();

        SetupScene();

        // Initialize Code Editor
        PythonEditor::Init();

        // Call Awake
        auto &awakeQueue = registry.ctx().get<EventSystem::AwakeQueue>();
        for (auto &fn : awakeQueue.functions) fn();
    }
    bool ShouldQuit() const {
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

    static void ShowPerformanceOverlay(const float shadowMS, const float mainMS) {
        ImGui::Begin("Pass Timers");

        ImGui::Text("GPU Shadow Pass: %.3f ms", shadowMS);
        ImGui::Text("GPU Main Pass:   %.3f ms", mainMS);
        ImGui::Text("Total GPU Time:  %.3f ms", shadowMS + mainMS);

        // Visual bar for relative cost
        const float total = shadowMS + mainMS;
        ImGui::ProgressBar(shadowMS / total, ImVec2(-1, 0), "Shadows");
        ImGui::ProgressBar(mainMS / total, ImVec2(-1, 0), "Main");

        ImGui::End();
    }

    void UI_Rendering(float shadowMS, float mainMS) {
        // UI here ------------------------------------------------------
        GUISystem::NewFrame();

        // Editor Windows
        // Sets the selected entity
        Hierarchy::Show(registry);
        const auto selectedObject = registry.ctx().get<Hierarchy::Hierarchy>().selectedEntity;
        if (selectedObject != entt::null) Inspector::Show(registry, selectedObject);
        else Inspector::Hide(registry);
        PythonEditor::Show(registry);
        ShowPerformanceOverlay(shadowMS, mainMS);
        Inspector::ShowDebugInfo();

        // Custom Windows
        auto &imguiQueue = registry.ctx().get<GUISystem::ImGuiDrawQueue>();
        for (auto &fn : imguiQueue) fn();

        // ImGui::Begin("Settings");
        // ImGui::Checkbox("RayTracing", &gameState->rayTracing);
        // ImGui::Checkbox("Playing", &gameState->isPlaying);
        // ImGui::End();

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
        PythonEditor::Destroy();
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

            // Update all matrices
            SimObject::UpdateHierarchyMatrices(registry, registry.ctx().get<SceneRoot>().entity, glm::mat4(1.0f));
            // --------------------------------------------------------------

            // Rendering --------------------------------------------------
            // Timers
            static GPUTimer shadowTimer, mainTimer;
            if (shadowTimer.queries[0] == 0) { shadowTimer.Init(); mainTimer.Init(); }

            // Shadow pass
            shadowTimer.Start();
            LightDepthPassSystem::BindFramebuffer(registry);
            LightDepthPassSystem::Render(registry);
            LightDepthPassSystem::UnbindFramebuffer();
            shadowTimer.Stop();

            // Main Pass
            // Anything inside framebuffer draws to the Scene window
            mainTimer.Start();
            RenderSystem::BindFramebuffer(registry);
            {
                // Ray Tracing
                if (gameState->rayTracing){
                    RT_Rendering();
                }
                else {
                    Normal_Rendering();
                }
            }
            RenderSystem::UnbindFramebuffer();
            mainTimer.Stop();
            // --------------------------------------------------------------

            // Push To Draw Queue -------------------------------------------
            // --------------------------------------------------------------

            UI_Rendering(shadowTimer.GetMS(), mainTimer.GetMS());

            // check and call events
            glfwPollEvents();
            glfwSwapBuffers(window.window);

            // Calling the Destroy Queue
            auto &entitiesToDelete = registry.ctx().get<EventSystem::DeleteQueue>().entities;
            for (const auto &entity : entitiesToDelete)
            {
                SimObject::Destroy(registry, entity);
            }
            entitiesToDelete.clear();

            // Clean up the selected Entity in the hierarchy
            auto &hierarchy = registry.ctx().get<Hierarchy::Hierarchy>();
            if(!registry.valid(hierarchy.selectedEntity)) hierarchy.selectedEntity = entt::null;
        }
        // ------------------------------------------------------------------
        CleanUp();
    }
};
