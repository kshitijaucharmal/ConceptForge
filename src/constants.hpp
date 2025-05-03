#pragma once

#include <string>

// Shader Paths
static std::string vertexShaderPath = std::string(SHADER_DIR) + "/engine.vert";
static std::string fragmentShaderPath = std::string(SHADER_DIR) + "/engine.frag";

// Constants
static const int WIDTH = 800;
static const int HEIGHT = 600;
static const float ASPECTRATIO = (float)WIDTH / (float)HEIGHT;
static const std::string WINDOWNAME = "ConceptForge";

const float cameraSpeed = 5.0f; // adjust accordingly
