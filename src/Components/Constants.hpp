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
    glm::vec4 CLEAR_COLOR{0.04f, 0.04f, 0.045f, 1.0f};

    int WINDOW_WIDTH = 1600;
    int WINDOW_HEIGHT = 900;
    float ASPECT_RATIO = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    std::string WINDOW_NAME = "ConceptForge";
    bool FULLSCREEN = false;

    // Editor
    // Inspector
    const int SCENE_X = 300;
    const int SCENE_Y = 0;
    const int SCENE_WIDTH = 992;
    const int SCENE_HEIGHT = 558;
};
