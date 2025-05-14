#include "vector"
#include "string"

#include "gui.hpp"
const char* glsl_version = "#version 130";

#include "constants.hpp"

namespace GUIManagement {
    MainGUI::MainGUI(GLFWwindow *window, ImGuiIO& IO) : io{IO}{
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        ImGuiStyle& style = ImGui::GetStyle();
        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can // look identical to regular ones. ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    GLfloat inspectorWidth = 350.0f;
    GLfloat inspectorHeight = HEIGHT;

    void MainGUI::NewFrame(){
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void MainGUI::RenderFrame(){
        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    // Windows -----------------------------------------------------------------------------
    void MainGUI::ShowInspectorWindow(SimObject::Entity& entity, ImGuizmo::OPERATION &operation, ImGuizmo::MODE &mode) {
        inspectorHeight = io.DisplaySize.y;
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - inspectorWidth, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(inspectorWidth, inspectorHeight), ImGuiCond_Always);

        ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        if (ImGui::CollapsingHeader("Gizmo Controls", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::RadioButton("Translate", operation == ImGuizmo::TRANSLATE)) operation = ImGuizmo::TRANSLATE;
            ImGui::SameLine();
            if (ImGui::RadioButton("Rotate", operation == ImGuizmo::ROTATE)) operation = ImGuizmo::ROTATE;
            ImGui::SameLine();
            if (ImGui::RadioButton("Scale", operation == ImGuizmo::SCALE)) operation = ImGuizmo::SCALE;

            if (ImGui::RadioButton("Local", mode == ImGuizmo::LOCAL)) mode = ImGuizmo::LOCAL;
            ImGui::SameLine();
            if (ImGui::RadioButton("Global", mode == ImGuizmo::WORLD)) mode = ImGuizmo::WORLD;
        }

        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            entity.GUI();
        }


        ImGui::End();
    }

    void MainGUI::ShowConsole() {
        ImGui::SetNextWindowPos(ImVec2(0, io.DisplaySize.y - 150.0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x - inspectorWidth, 150.0), ImGuiCond_Always);

        ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        ImGui::Text("ImGuizmo IsOver: %s", ImGuizmo::IsOver() ? "true" : "false");
        ImGui::Text("ImGuizmo IsUsing: %s", ImGuizmo::IsUsing() ? "true" : "false");
        ImGui::Text("Mouse Position: %.1f, %.1f", io.MousePos.x, io.MousePos.y);
        ImGui::Text("ImGui WantCaptureMouse: %s", io.WantCaptureMouse ? "true" : "false");
        ImGui::End();
    }

    // -------------------------------------------------------------------------------------

    MainGUI::~MainGUI(){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}
