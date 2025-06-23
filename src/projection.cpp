
#include "projection.hpp"
#include "constants.hpp"

Projection::Projection() {
    projection = glm::mat4(1.0);
    view = glm::mat4(1.0);
}

void Projection::Calculate(Camera &camera){
    // Projection Matrix
    projection = glm::perspective(glm::radians(camera.fov), Const::ASPECTRATIO, 0.01f, 100.0f);
    view = camera.GetViewMatrix();
}

void Projection::UpdateShader(ShaderManagement::ShaderProgram &sp){
    sp.Use();
    sp.setMat4("projection", projection);
    sp.setMat4("view", view);
}

