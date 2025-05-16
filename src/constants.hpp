#pragma once

#include <string>

static const char* glsl_version = "#version 130";

// Shader Paths
static std::string vertexShaderPath = std::string(SHADER_DIR) + "/engine.vert";
static std::string fragmentShaderPath = std::string(SHADER_DIR) + "/engine.frag";

// Constants
static const int WIDTH = 1600;
static const int HEIGHT = 900;
static const float ASPECTRATIO = (float)WIDTH / (float)HEIGHT;
static const std::string WINDOWNAME = "ConceptForge";

const float cameraSpeed = 5.0f; // adjust accordingly

// Editor
// Inspector
static const int inspectorWidth = 350; // height determined by window size
static const int consoleHeight = 150;
static const int assetBrowserWidth = 300;
