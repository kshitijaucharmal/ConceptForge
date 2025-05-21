#include "setup.hpp"

using namespace Engine;

ConceptForge::ConceptForge():
  camera(glm::vec3(0.0f, 0.0f, 3.0f)),
  window(Const::WIDTH, Const::HEIGHT, Const::WINDOWNAME, false),
  shaderProgram(DrawMode::FILLED, Const::vertexShaderPath, Const::fragmentShaderPath),
  mainGui(window.window)
{
    // Projection Logic - Initialize
    Projection projection;

    // Bind all textures
    shaderProgram.BindTextures();

    // Setup Main GUI
    Editor::Gizmo gizmo;
    Editor::Inspector inspector;
    Editor::AssetBrowser asset_browser;

    // Cube cube(shaderProgram);
}

ConceptForge::~ConceptForge(){

}

void ConceptForge::DeltaTimeCalc(){
    // Delta Time calculation
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
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

}
void ConceptForge::CalcProjection(){
    projection.Calculate(camera, shaderProgram);
}

void ConceptForge::GUIManagement(){
    // Remove from here
    // Send Shader Data
    // cube.Draw();

    // check and call events
    glfwPollEvents();

    // Imgui
    if (glfwGetWindowAttrib(window.window, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      return;
    }

    // Create new Frame
    mainGui.NewFrame();

    // Draw All UI
    mainGui.ShowConsole();
    // gizmo.Show(cube, projection, camera);
    // inspector.Show(cube, gizmo.gizmoOperation, gizmo.gizmoMode);
    asset_browser.Show(Const::projectDir);

    // Render
    mainGui.RenderFrame();

    glfwSwapBuffers(window.window);
}
