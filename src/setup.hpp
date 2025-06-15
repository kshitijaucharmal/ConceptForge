#pragma once

// Includes GLFW and glad
#include "window.hpp"

#include "input.hpp"

#include "shaderman.hpp"

#include <string>
#include <memory>
#include <unordered_map>

// GLM
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include "glm/gtx/matrix_decompose.hpp"

#include "constants.hpp"
#include "projection.hpp"

// Imgui Based UI
#include "editor/gui.hpp"

#include "primitives/entity.hpp"

#include "editor/asset_browser.hpp"
#include "editor/hierarchy.hpp"
#include "editor/gizmo.hpp"
#include "editor/inspector.hpp"
#include "editor/object_creation_menu.hpp"

using namespace ShaderManagement;

namespace Engine {
    class ConceptForge{
    public:
        // Time management
        float lastFrame = 0.0f; // Time of last frame
        // Projection Logic
        Camera camera;
        WindowManagement::Window window;
        std::unordered_map<ShaderType, std::shared_ptr<ShaderProgram>> shaders;
        InputManagement::Input input;

        // GUI
        Editor::MainGUI mainGui;
        Editor::Gizmo gizmo;
        Editor::Inspector inspector;
        // Editor::AssetBrowser asset_browser;
        Editor::Hierarchy hierarchy;
        Editor::ObjectCreationMenu objCreatorMenu;
        Projection projection;

        ConceptForge();
        ~ConceptForge();

        float deltaTime = 0.0f; // Time between current frame and last frame

        // Seperate functions
        void SetupShaders();
        bool WindowShouldClose();
        float DeltaTimeCalc();
        void ProcessInput();
        void Render();
        void CalcProjection();
        void GUIManagement();
    };
}
