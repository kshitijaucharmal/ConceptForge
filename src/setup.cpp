#include "setup.hpp"

using namespace Engine;

ConceptForge::ConceptForge():
  camera(glm::vec3(7.3589f, 5.3444f, 6.9258f), glm::vec3(0.0f, 1.0f, 0.0f), -132.4f, -28.2f),
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

    Cube cube(shaderProgram);
    cubes.push_back(cube);
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

void ConceptForge::SetSelected(int selected){
    if(selected < 0 || selected >= cubes.size()){
        std::cout << "Selection is wrong" << std::endl;
    }
    selectedCube = selected;
}

void ConceptForge::GUIManagement(){
    // Remove from here
    // Send Shader Data
    for(Cube cube : cubes) cube.Draw();

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
    if(selectedCube != -1){
        gizmo.Show(cubes[selectedCube], projection, camera);
        inspector.Show(cubes[selectedCube], gizmo.gizmoOperation, gizmo.gizmoMode);
    }
    asset_browser.Show(Const::projectDir);

    // Render
    mainGui.RenderFrame();

    glfwSwapBuffers(window.window);
}
