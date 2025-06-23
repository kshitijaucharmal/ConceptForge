#include "setup.hpp"

using namespace Engine;
using namespace ShaderManagement;

ConceptForge::ConceptForge():
  camera(glm::vec3(7.3589f, 7.3444f, 6.9258f), glm::vec3(0.0f, 1.0f, 0.0f), -132.4f, -28.2f),
  window(Const::WIDTH, Const::HEIGHT, Const::WINDOWNAME, false),
  mainGui(window.window) {
    // Projection Logic - Initialize
    Projection projection;

    // Creating the SSBO at startup
    glGenBuffers(1, &pointLightSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, pointLightSSBO); // match binding = 1

    glGenBuffers(1, &dirLightSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, dirLightSSBO);

    pointLights = {
        PointLight(glm::vec3(0), glm::vec3(0.05), glm::vec3(2.0), glm::vec3(2.0)),
        // PointLight(glm::vec3(0), glm::vec3(0.05), glm::vec3(3.0, 0.0, 0.0), glm::vec3(3.0, 0.0, 0.0)),
        // PointLight(glm::vec3(0), glm::vec3(0.05), glm::vec3(0, 3.0, 0), glm::vec3(0., 3.0, 0)),
        // PointLight(glm::vec3(0), glm::vec3(0.05), glm::vec3(0., 0., 3.0), glm::vec3(0., 0., 3.0))
    };

    dirLights = {
        DirectionalLight(glm::vec3(-34.f, -1.0f, -0.3f), glm::vec3(0.1f), glm::vec3(0.4f), glm::vec3(0.5f))
    };

    // Setup Main GUI
    Editor::Gizmo gizmo;
    Editor::Inspector inspector;
    // Editor::AssetBrowser asset_browser;
    Editor::Hierarchy hierarchy;
    Editor::ObjectCreationMenu objCreatorMenu;
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
    glClearColor(Const::clearColor.x, Const::clearColor.y, Const::clearColor.z, Const::clearColor.w);
    // Clear Color Buffer and Depth Buffer
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Point Lights
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, pointLightSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, pointLights.size() * sizeof(PointLight), pointLights.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Directional Lights
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, dirLightSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dirLights.size() * sizeof(DirectionalLight), dirLights.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


    // Remove from here
    // Send Shader Data
    for(const auto& entity : hierarchy.entities) {
        entity.second->Draw();
    }
}

void ConceptForge::CalcProjection(){
    // Caculate once
    projection.Calculate(camera);
    // Circulate to all materials (and in turn all their shaders)
    // TODO: Make a function in each entity for this, this is too crude
    for(const auto& entity : hierarchy.entities ){
        for(auto &material : entity.second->materials){
            material->ApplyProjection(projection);
        }
    }
}

void ConceptForge::GUIManagement(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

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

    #define ICON_FA_CUBE "\xef\x86\xb2"        // Filled cube
    #define ICON_FA_SQUARE "\xef\x83\x88"      // Square (wireframe-like)

    ImVec2 iconSize = ImVec2(32, 32);  // Square button
    float fontSize = ImGui::GetFontSize();  // Usually ~16–20

    // Calculate text size
    ImVec2 textSize = ImGui::CalcTextSize(ICON_FA_CUBE);

    // Calculate offset to center icon inside the button
    ImVec2 textOffset = ImVec2(
        (iconSize.x - textSize.x) * 0.5f, (iconSize.y - textSize.y) * 0.5f
    );

    if (ImGui::Button("##squareBtn", iconSize)) {
        // Handle click
        // for(auto const &material : materials){
        //     material.second->SetDrawMode(DrawMode::FILLED);
        // }
    }
    // Manually draw the icon centered over the button
    ImVec2 pos = ImGui::GetItemRectMin();
    ImGui::GetWindowDrawList()->AddText(
        ImGui::GetFont(), fontSize, ImVec2(pos.x + textOffset.x, pos.y + textOffset.y),
                                        ImGui::GetColorU32(ImGuiCol_Text),
                                        ICON_FA_SQUARE
    );
    ImGui::SameLine();
    if (ImGui::Button("##cubeBtn", iconSize)) {
        // Handle click
        // for(auto const &material : materials){
        //     material.second->SetDrawMode(DrawMode::WIREFRAME);
        // }
    }

    // Manually draw the icon centered over the button
    pos = ImGui::GetItemRectMin();
    ImGui::GetWindowDrawList()->AddText(
        ImGui::GetFont(), fontSize, ImVec2(pos.x + textOffset.x, pos.y + textOffset.y),
                                        ImGui::GetColorU32(ImGuiCol_Text),
                                        ICON_FA_CUBE
    );
    ImGui::SameLine();
    ImGui::Text("Draw Mode");

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
ConceptForge::~ConceptForge(){

}
