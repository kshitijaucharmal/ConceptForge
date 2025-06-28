#include <entt/entt.hpp>

#include "Core/WindowManager.hpp"
#include "Core/GUISystem.hpp"
#include "Core/EventSystem.hpp"

#include "Components/Camera.hpp"
#include "Components/Constants.hpp"
#include "Components/Time.hpp"
#include "Components/Rendering/FrameBuffer.hpp"
#include "Components/Rendering/GizmoControls.hpp"

#include "Systems/CameraSystem.hpp"
#include "Systems/TimeSystem.hpp"
#include "Systems/Rendering/MaterialSystem.hpp"
#include "Systems/Rendering/ShaderSystem.hpp"
#include "Systems/Rendering/RenderSystem.hpp"
#include "Systems/Primitives/CubeSystem.hpp"
#include "Systems/Primitives/GizmoSystem.hpp"

#include "Core/EditorWindows/Inspector.hpp"
#include "Core/EditorWindows/Hierarchy.hpp"
#include "Systems/Primitives/GridSystem.hpp"

int main(){
    entt::registry registry;

    // Setup Global Constants (Might be changed)
    auto constants = registry.ctx().emplace<Constants>();

    // Global Values (Context)
    registry.ctx().emplace<Hierarchy::Hierarchy>();
    registry.ctx().emplace<Time>();
    registry.ctx().emplace<ActiveCamera>();
    registry.ctx().emplace<EventSystem::AwakeQueue>();
    registry.ctx().emplace<EventSystem::UpdateQueue>();
    registry.ctx().emplace<EventSystem::LateUpdateQueue>();
    registry.ctx().emplace<MaterialSystem::WhiteTexture>();
    registry.ctx().emplace<GUISystem::ImGuiDrawQueue>();
    registry.ctx().emplace<FrameBuffer>();
    registry.ctx().emplace<GizmoControls>();

    // Window -----------------------------------------------------------
    // initialize window (and OpenGL)
    Window window(registry, 1600, 900, "ConceptForge");
    // Initialize the FrameBuffer
    RenderSystem::Init(registry);
    // ------------------------------------------------------------------

    // Camera ----------------------------------------------------------
    // Create camera (Can be many, but one for now)
    const auto camera = CameraSystem::CreateCamera(registry, "Main Camera");

    // Set it as the active camera
    registry.ctx().insert_or_assign<ActiveCamera>({camera});
    // ------------------------------------------------------------------

    // Shader System ----------------------------------------------------
    // Shaders
    entt::entity gridShader = registry.create();
    registry.emplace<Shader>(gridShader, Shader{
        .vertexShaderPath = SHADER_DIR "/grid.vert",
        .fragmentShaderPath = SHADER_DIR "/grid.frag"
    });

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

    // Grid
    auto grid = GridSystem::CreateGrid(registry, gridShader, "Grid");

    // Cube Objects
    auto cube1 = CubeSystem::CreateCubeObject(registry,
        Transform{
            .name = "Cube 1",
            .position = glm::vec3(2.0, 2.0, 0.0)
        }, litShader);
    auto cube2 = CubeSystem::CreateCubeObject(registry,
        Transform{
            .name = "Cube 2",
            .position = glm::vec3(-2.0, 2.0, 0.0)
    }, litShader);
    auto cube3 = CubeSystem::CreateCubeObject(registry, Transform{.name = "Cube 3"}, unlitShader);
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
        CameraSystem::CalculateProjection(registry);

        // Call every System in Update/LateUpdate
        // Update
        auto &updateQueue = registry.ctx().get<EventSystem::UpdateQueue>();
        for (auto &fn : updateQueue.functions) fn();

        // Late Update
        auto &lateUpdateQueue = registry.ctx().get<EventSystem::LateUpdateQueue>();
        for (auto &fn : lateUpdateQueue.functions) fn();
        // --------------------------------------------------------------

        // Before drawing anything, clear screen
        Window::ScreenClearFlags(registry);

        // Render here --------------------------------------------------
        // Anything inside framebuffer draws to the Scene window
        RenderSystem::BindFramebuffer(registry);

        RenderSystem::Render(registry);
        GridSystem::Render(registry, grid, registry.get<Shader>(gridShader));

        RenderSystem::UnbindFramebuffer();
        // --------------------------------------------------------------

        // Push To Draw Queue -------------------------------------------
        // --------------------------------------------------------------

        // UI here ------------------------------------------------------
        GUISystem::NewFrame();

        RenderSystem::ShowSceneTexture(registry);
        // Editor Windows
        Hierarchy::Show(registry);
        Inspector::Show(registry);

        // Custom Windows
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
