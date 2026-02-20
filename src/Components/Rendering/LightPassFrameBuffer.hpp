//
// Created by kshitij on 2/20/26.
//

#pragma once
#include <glad/glad.h>

/**
 * Store the Depth Buffer for all lights (Shadow Atlas)
 * Stored as texture from all lights in scene
 */
struct LightPassFrameBuffer
{
    GLuint frameBufferID = 0;
    GLuint shadowDepthArray = 0;

    // Texture dimensions
    int width = 1024;
    int height = 1024;
};