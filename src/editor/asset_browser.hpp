#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <filesystem>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

#include "imgui.h"
#include "stb_image.h"
#include "constants.hpp"

#include "IconsFontAwesome6.h"

namespace Editor {
    class AssetBrowser {
    public:
        AssetBrowser();
        GLuint LoadTexture(const std::string& filePath);
        void Show(std::string& currentDirectory);
    };
}
