#pragma once

#include <glad/glad.h>

#include <vector>
#include <unordered_map>
#include <memory>

#include "shaderman.hpp"
#include "constants.hpp"
#include "projection.hpp"

using namespace ShaderManagement;

class Material {
public:

    std::shared_ptr<ShaderProgram> shader;

    // Default parameters
    unsigned int whiteTexture;
    unsigned int diffuseMap;
    unsigned int specularMap;

    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;

    // Functions
    Material(ShaderType type=ShaderType::Lit);
    void InitWhiteTexture();
    void InitShader(ShaderType type);
    void InitDefaultLit();
    void InitDefaultUnlit();
    void InitDefaultLight();

    void SetDrawMode(DrawMode mode);

    void ApplyProjection(Projection &projection);
};
