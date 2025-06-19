#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Const {
    static const char* glsl_version = "#version 130";

    // Shader Paths
    // Shouldn't be constant I think
    static std::string unlitVert = std::string(SHADER_DIR) + "/unlit.vert";
    static std::string unlitFrag = std::string(SHADER_DIR) + "/unlit.frag";
    static std::string litVert = std::string(SHADER_DIR) + "/lit.vert";
    static std::string litFrag = std::string(SHADER_DIR) + "/lit.frag";
    static std::string lightVert = std::string(SHADER_DIR) + "/light.vert";
    static std::string lightFrag = std::string(SHADER_DIR) + "/light.frag";
    // Darkish blue
    static glm::vec4 clearColor(0.03, 0.03, 0.035, 1.0f);

    // Constants
    static const int WIDTH = 1600;
    static const int HEIGHT = 900;
    static const float ASPECTRATIO = (float)WIDTH / (float)HEIGHT;
    static const std::string WINDOWNAME = "ConceptForge";

    const float cameraSpeed = 5.0f; // adjust accordingly

    // Editor
    // Inspector
    static const int inspectorWidth = 350; // height determined by window size
    static const int consoleHeight = 500;
    static const int assetBrowserWidth = 300;

    // TODO: make this absolute path
    static std::string projectDir = "/mnt/Data/projects/Graphics/OpenGL";
}
