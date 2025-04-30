#pragma once

#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include <glm/vec4.hpp>

namespace GUIManagement {
    class MainGUI {
    private:
    public:
        ImGuiIO& io;
        MainGUI(GLFWwindow *window, ImGuiIO& IO);
        ~MainGUI();

        void NewFrame();
        void RenderFrame();
        // Just demo
        void DemoWindow(glm::vec4 clearColor);
    };
}
