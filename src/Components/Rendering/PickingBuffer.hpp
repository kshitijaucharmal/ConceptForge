//
// Created by kshitij on 2/21/26.
//

#ifndef CONCEPTFORGE_PICKINGBUFFER_HPP
#define CONCEPTFORGE_PICKINGBUFFER_HPP
#include "glad/glad.h"

struct PickingBuffer
{
    int frameBufferID = 0;
    GLuint colorTexture = 0;
    GLuint depthBuffer = 0;
};

#endif //CONCEPTFORGE_PICKINGBUFFER_HPP