#pragma once

// Includes GLFW and glad
#include "window.hpp"

#include "input.hpp"

#include "shaderman.hpp"

#include <string>

// GLM
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include "glm/gtx/matrix_decompose.hpp"

#include "constants.hpp"
#include "projection.hpp"

// Imgui Based UI
#include "editor/gui.hpp"

#include "primitives/cube.hpp"

#include "editor/asset_browser.hpp"
#include "editor/gizmo.hpp"
#include "editor/inspector.hpp"

namespace Engine {
    class ConceptForge{
    private:
        // Time management
        float lastFrame = 0.0f; // Time of last frame
        // Projection Logic
        Camera camera;
        WindowManagement::Window window;
        ShaderManagement::ShaderProgram shaderProgram;
        InputManagement::Input input;

        // GUI
        Editor::MainGUI mainGui;
        Editor::Gizmo gizmo;
        Editor::Inspector inspector;
        Editor::AssetBrowser asset_browser;
        Projection projection;

    public:
        ConceptForge();
        ~ConceptForge();

        // Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
        float deltaTime = 0.0f; // Time between current frame and last frame

        // Seperate functions
        bool WindowShouldClose();
        void DeltaTimeCalc();
        void ProcessInput();
        void Render();
        void CalcProjection();
        void GUIManagement();
    };
}
