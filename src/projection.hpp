#pragma once

#include "glm/glm.hpp"
#include "camera.hpp"
#include "shaderman.hpp"

// Class to handle all projection - view - model matrix operations
class Projection {
public:
    // Variables
    // TODO: Setter/Getter
    glm::mat4 projection;
    glm::mat4 view;

    // Constructor
    Projection();

    void Calculate(Camera &camera);
    void UpdateShader(ShaderManagement::ShaderProgram &sp);
};

