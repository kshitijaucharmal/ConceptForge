#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>

#include "utilities/fileloader.hpp"
#include "stb_image.h"

class TextureManager {
public:
    TextureManager();

    std::unordered_map<std::string, uint> allTextures;

    // Functions
    void BindTexture(uint shaderID, uint textureID, uint textureLoc, const char* textureShaderName);
    uint LoadTexture(const char* texturePath, bool flip=true);
};
