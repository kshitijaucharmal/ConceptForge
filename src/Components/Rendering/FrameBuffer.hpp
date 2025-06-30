#pragma once

#include <glad/glad.h>

struct FrameBuffer{
    GLuint frameBufferID = 0;
    GLuint colorTexture = 0;
    GLuint depthBuffer = 0;
};
