#pragma once
#include <string>

// ShaderType
enum ShaderType {
    UNLIT,
    LIT,
    LIGHT
};

enum DrawMode {
    WIREFRAME,
    FILLED
};

struct Shader {
    std::string vertexShaderPath;
    std::string fragmentShaderPath;

    bool initialized = false;

    // IDs Given by OpenGL
    uint shaderID = 0;
    uint vertexShader = 0;
    uint fragmentShader = 0;

    DrawMode drawMode = DrawMode::FILLED;
    ShaderType type = ShaderType::LIT;
};
