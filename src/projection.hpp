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
    GLfloat fov = 85.0f;

    // Constructor
    Projection();

    void Calculate(Camera &camera, ShaderManagement::ShaderProgram &shaderProgram);
};

