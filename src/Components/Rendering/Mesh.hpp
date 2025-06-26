#pragma once

#include <entt/entt.hpp>
#include <glad/glad.h>

struct Mesh {
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLsizei indexCount = 0;
    bool initialized = false;
};
