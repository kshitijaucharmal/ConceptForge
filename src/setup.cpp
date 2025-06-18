#include "setup.hpp"

#include "primitives/uv_sphere.hpp"
#include "primitives/cube.hpp"

using namespace Engine;
using namespace ShaderManagement;

ConceptForge::ConceptForge():
  camera(glm::vec3(7.3589f, 7.3444f, 6.9258f), glm::vec3(0.0f, 1.0f, 0.0f), -132.4f, -28.2f),
  window(Const::WIDTH, Const::HEIGHT, Const::WINDOWNAME, false),
  mainGui(window.window)
{
    // Projection Logic - Initialize
    Projection projection;

    SetupShaders();

    // Setup Main GUI
    Editor::Gizmo gizmo;
    Editor::Inspector inspector;
    // Editor::AssetBrowser asset_browser;
    Editor::Hierarchy hierarchy;
    Editor::ObjectCreationMenu objCreatorMenu;
}

void ConceptForge::SetupShaders(){
    // Lit Shader
    std::shared_ptr<ShaderProgram> litShader = std::make_shared<ShaderProgram>();
    litShader->Init(DrawMode::FILLED, Const::litVert, Const::litFrag);
    litShader->BindTexture(TEXTURE_DIR "/container2.png", "texture1", 0, true);
    litShader->BindTexture(TEXTURE_DIR "/container2_specular.png", "texture2", 1, false);
    litShader->setInt("material.diffuse", 0);
    litShader->setInt("material.specular", 1);
    shaders[ShaderType::Lit] = std::move(litShader);

    // Unlit Shader
    std::shared_ptr<ShaderProgram> unlitShader = std::make_shared<ShaderProgram>();
    unlitShader->Init(DrawMode::WIREFRAME, Const::unlitVert, Const::unlitFrag);
    unlitShader->Use();
    // unsigned int texture1 = unlitShader->BindTexture(TEXTURE_DIR "/container2.png", "texture1", 0, true);
    // unsigned int texture2 = unlitShader->BindTexture(TEXTURE_DIR "/container2_specular.png", "texture2", 1, false);

    shaders[ShaderType::Unlit] = std::move(unlitShader);

    // Light Shader (Shader for a light emitting object)
    std::shared_ptr<ShaderProgram> lightShader = std::make_shared<ShaderProgram>();
    lightShader->Init(DrawMode::FILLED, Const::lightVert, Const::lightFrag);
    shaders[ShaderType::Light] = std::move(lightShader);
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

    shaders[ShaderType::Lit]->Use();
    // Set the colors here
    shaders[ShaderType::Lit]->setVec3("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));
    shaders[ShaderType::Lit]->setVec3("objectColor", glm::vec3(0.0f, 1.0f, 0.31f));

    window.RenderToFBO();
}

void ConceptForge::CalcProjection(){
    // WARNING Something is off here
    for(auto &shader : shaders){
        projection.Calculate(camera, *shader.second);
    }
}

void ConceptForge::GUIManagement(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
    // Remove from here
    // Send Shader Data
    for(const auto& entity : hierarchy.entities) {
        entity.second->Draw();
    }

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
        for(auto const &shader : shaders){
            shader.second->SetDrawMode(DrawMode::FILLED);
        }
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
        for(auto const &shader : shaders){
            shader.second->SetDrawMode(DrawMode::WIREFRAME);
        }
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
