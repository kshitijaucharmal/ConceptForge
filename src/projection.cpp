
#include "projection.hpp"
#include "constants.hpp"

Projection::Projection() {
    projection = glm::mat4(1.0);
    view = glm::mat4(1.0);
}

void Projection::Calculate(Camera &camera, ShaderManagement::ShaderProgram &shaderProgram) {
    // Use a ShaderProgram
    shaderProgram.Use();

    // Projection Matrix
    projection = glm::perspective(glm::radians(fov), Const::ASPECTRATIO, 0.01f, 100.0f);
    shaderProgram.setMat4("projection", projection);

    // Camera
    // Vector from position to origin
    view = camera.GetViewMatrix();
    shaderProgram.setMat4("view", view);
}

