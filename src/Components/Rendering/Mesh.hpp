#pragma once

#include <entt/entt.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>

// Order matters here
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    GLsizei indexCount = 0;
    // DrawMode::Elements or Arrays
    bool elemental = false;
    bool initialized = false;
};
