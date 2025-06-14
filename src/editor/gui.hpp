#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/vec4.hpp>

#include "ImGuizmo.h"

#include "vector"
#include "string"

#include "constants.hpp"
#include "gizmo.hpp"

namespace Editor {
    class MainGUI {
    private:
        GLboolean b_inspector = true;
        GLboolean b_assetBrowser = true;
        GLboolean b_debugConsole = true;
        GLboolean b_cameraControls = true;
        GLboolean b_gizmos = true;
    public:
        ImGuiIO io;
        MainGUI(GLFWwindow* window);
        ~MainGUI();

        void NewFrame();
        void RenderFrame();

        void ImguiBegin();
        void ImguiEnd();

        void SetupEditorWindows();
        void DrawEditorWindows();

        // Editor Windows
        void ShowCameraControls(GLfloat *fov);
        void ShowConsole();

        void Show();
    };
}
