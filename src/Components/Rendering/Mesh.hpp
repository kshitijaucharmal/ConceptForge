#pragma once

#include <entt/entt.hpp>
#include <glad/glad.h>

struct Mesh {
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    GLsizei indexCount = 0;
    bool elemental = false;
    bool initialized = false;
};
