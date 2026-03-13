//
// Created by kshitij on 3/12/26.
//

#ifndef CONCEPTFORGE_POSTPROCESSCONTEXT_HPP
#define CONCEPTFORGE_POSTPROCESSCONTEXT_HPP

#include "glad/glad.h"
#include <vector>
#include <string>

struct PostProcessContext {
    GLuint pingPongFBOs[2]      = {0, 0};
    GLuint pingPongTextures[2]  = {0, 0};
    GLuint outputTexture;
    GLuint quadVAO;

    GLuint renderWidth = 320;
    GLuint renderHeight = 240;

    // Ordered list of shader keys — push/pop to enable/disable effects
    std::vector<std::string> activePasses = {
        "PixelartShader"
    };
};

#endif //CONCEPTFORGE_POSTPROCESSCONTEXT_HPP