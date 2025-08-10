#pragma once
#include <glm/glm.hpp>

struct Grid {
    int size = 50;             // Number of lines in each direction
    float spacing = 5.0f;      // Distance between lines
    glm::vec3 color = glm::vec3(0.5f);
    bool infinite = true;     // Should grid follow camera?
};