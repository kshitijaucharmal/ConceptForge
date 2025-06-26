#pragma once

#include <entt/entt.hpp>
#include <glad/glad.h>
#include <string>

struct Mesh {
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLsizei indexCount = 0;
    bool initialized = false;
};

struct MeshFilter {
    entt::entity meshEntity = entt::null;
};

struct MeshRenderer {};
