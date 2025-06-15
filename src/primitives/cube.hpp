#pragma once

// Extend from entity
#include "entity.hpp"
#include "shaderman.hpp"

class Cube : public SimObject::Entity {
private:
    GLuint VAO;
    GLuint VBO;

public:
    Cube(ShaderManagement::ShaderProgram *sp, glm::vec3 pos=glm::vec3(0.0), glm::vec3 rot=glm::vec3(0.0), glm::vec3 sca=glm::vec3(1.0));
    void SetupVAO();
    void SetupVBO();
    void SetupVertexData();

    void Draw() override;
};
