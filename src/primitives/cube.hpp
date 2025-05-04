#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>
// GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Cube {
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int shaderProgram;


public:
    // Transform Components
    glm::vec3 position;
    // TODO: Handle rot and scaling
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 model = glm::mat4(1.0f);
    Cube(unsigned int sp);
    void Reset();

    void Translate(glm::vec3 pos);
    void Rotate(float angle, glm::vec3 axis);

    void Use();
    void Update();
    void Draw();
};
