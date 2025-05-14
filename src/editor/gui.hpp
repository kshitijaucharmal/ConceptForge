#pragma once

#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/vec4.hpp>

// Entity
#include "../primitives/entity.hpp"

#include "ImGuizmo.h"

namespace GUIManagement {
    class MainGUI {
    private:
        GLboolean b_inspector = true;
        GLboolean b_fileSystem = true;
        GLboolean b_debug_console = true;
        GLboolean b_gizmos = true;
    public:
        ImGuiIO& io;
        MainGUI(GLFWwindow *window, ImGuiIO& IO);
        ~MainGUI();

        void NewFrame();
        void RenderFrame();

        void SetupEditorWindows();
        void DrawEditorWindows();

        // Editor Windows
        void ShowInspectorWindow(SimObject::Entity &entity, ImGuizmo::OPERATION &operation, ImGuizmo::MODE &mode);
        void ShowConsole();
        void ShowFileSystem();
    };
}
