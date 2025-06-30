#pragma once
#include <glm/glm.hpp>

struct Grid {
    int size = 50;             // Number of lines in each direction
    float spacing = 1.0f;      // Distance between lines
    glm::vec3 color = glm::vec3(0.4f);
    bool infinite = false;     // Should grid follow camera?
};