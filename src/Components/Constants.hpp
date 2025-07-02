#pragma once

#include <string>
#include <glm/glm.hpp>

struct Constants{
    const char* GLSL_VERSION = "#version 130";

    // Shader Paths
    // Shouldn't be constant I think
    std::string SP_UNLIT_VERT = std::string(SHADER_DIR) + "/unlit.vert";
    std::string SP_UNLIT_FRAG = std::string(SHADER_DIR) + "/unlit.frag";
    std::string SP_LIT_VERT = std::string(SHADER_DIR) + "/lit.vert";
    std::string SP_LIT_FRAG = std::string(SHADER_DIR) + "/lit.frag";
    std::string SP_LIGHT_VERT = std::string(SHADER_DIR) + "/light.vert";
    std::string SP_LIGHT_FRAG = std::string(SHADER_DIR) + "/light.frag";

    // Darkish blue
    glm::vec4 BACKGROUND_COLOR{0.04f, 0.04f, 0.045f, 1.0f};
    glm::vec4 CLEAR_COLOR{0.5, 0.7, 1.0, 1.0f};

    int WINDOW_WIDTH = 1600;
    int WINDOW_HEIGHT = 900;
    float ASPECT_RATIO = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
    std::string WINDOW_NAME = "ConceptForge";
    bool FULLSCREEN = false;
    bool MouseCaptured = false;

    // Editor
    // Inspector
    const int SCENE_X = 302;
    const int SCENE_Y = 2;
    const int SCENE_WIDTH = 992;
    const int SCENE_HEIGHT = 558;
};
