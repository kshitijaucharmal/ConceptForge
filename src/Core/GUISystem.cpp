#include "GUISystem.hpp"

#include "Components/Constants.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace GUISystem {
void InitImGUI(entt::registry &registry, GLFWwindow* window) {
    const auto constants = registry.ctx().get<Constants>();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    auto io = ImGui::GetIO();

    // Config Flags
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(constants.GLSL_VERSION);

    ImFontConfig config;
    config.MergeMode = true;
    config.PixelSnapH = true;

    // Load main font
    io.Fonts->AddFontFromFileTTF(FONT_DIR "/TextFont.ttf", 16.0f);

    // Load icon font (e.g., FontAwesome)
    static constexpr ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
    io.Fonts->AddFontFromFileTTF(FONT_DIR "/fa-solid-900.ttf", 16.0f, &config, icons_ranges);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
}

void NewFrame(){
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void RenderFrame(){
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (const auto io = ImGui::GetIO(); io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void PushToDrawQueue(entt::registry& registry, std::function<void ()> fn){
    auto &queue = registry.ctx().get<ImGuiDrawQueue>();
    queue.emplace_back(std::move(fn));
}

void Destroy(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


}
