#include "setup.hpp"

#include "primitives/uv_sphere.hpp"
#include "primitives/cube.hpp"

using namespace Engine;

ConceptForge::ConceptForge():
  camera(glm::vec3(7.3589f, 5.3444f, 6.9258f), glm::vec3(0.0f, 1.0f, 0.0f), -132.4f, -28.2f),
  window(Const::WIDTH, Const::HEIGHT, Const::WINDOWNAME, false),
  mainGui(window.window)
{
    // Projection Logic - Initialize
    Projection projection;

    shaderProgram.Init(DrawMode::FILLED, Const::vertexShaderPath, Const::fragmentShaderPath),
    // Bind all textures
    shaderProgram.BindTextures();

    // Setup Main GUI
    Editor::Gizmo gizmo;
    Editor::Inspector inspector;
    // Editor::AssetBrowser asset_browser;
    Editor::Hierarchy hierarchy;
    Editor::ObjectCreationMenu objCreatorMenu;

    // Add a cube
    std::unique_ptr<Cube> cube = std::make_unique<Cube>(shaderProgram);
    hierarchy.AddEntity(std::move(cube));
}

ConceptForge::~ConceptForge(){

}

float ConceptForge::DeltaTimeCalc(){
    // Delta Time calculation
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    return deltaTime;
}

bool ConceptForge::WindowShouldClose() {
    return glfwWindowShouldClose(window.window);
}


void ConceptForge::ProcessInput(){
    // input processing
    input.processInput(window.window, camera, deltaTime);
}
void ConceptForge::Render(){
    // rendering commands
    // Clear Screen with this color
    auto clearColor = Const::clearColor;
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    window.RenderToFBO();
}

void ConceptForge::CalcProjection(){
    projection.Calculate(camera, shaderProgram);
}

void ConceptForge::GUIManagement(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
    // Remove from here
    // Send Shader Data
    for(const auto& entity : hierarchy.entities) entity.second->Draw();

    // check and call events
    glfwPollEvents();

    // Imgui
    if (glfwGetWindowAttrib(window.window, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      return;
    }

    // Create new Frame
    mainGui.NewFrame();

    // window.RenderToImGui();
    // objCreatorMenu.Show();

    mainGui.ImguiBegin();
    mainGui.ShowConsole();
    mainGui.ShowCameraControls(&camera.fov);
    mainGui.ImguiEnd();

    // Draw All UI
    SimObject::Entity *current_entity = hierarchy.GetSelected();
    gizmo.Show(current_entity, projection, camera);
    inspector.Show(current_entity, gizmo.gizmoOperation, gizmo.gizmoMode);

    hierarchy.Show();
    // Render
    mainGui.RenderFrame();

    glfwSwapBuffers(window.window);
}
