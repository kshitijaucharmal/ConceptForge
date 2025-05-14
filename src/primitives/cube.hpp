#pragma once

// Extend from entity
#include "entity.hpp"

class Cube : public SimObject::Entity {
private:
    GLuint VAO;
    GLuint VBO;
    GLuint shaderProgram;

public:
    Cube(unsigned int sp, glm::vec3 pos=glm::vec3(0.0), glm::vec3 rot=glm::vec3(0.0), glm::vec3 sca=glm::vec3(1.0));
    void SetupTextures();

    // Transform ---------------------------------------------------------------
    void Translate(glm::vec3 pos) override;
    void Rotate(glm::vec3 rot) override;
    void Rotate(float angle, glm::vec3 axis) override;
    void Scale(glm::vec3 factor) override;
    void UpdateModelMatrix() override;
    // -------------------------------------------------------------------------

    void Draw() override;
    void GUI() override;
};
