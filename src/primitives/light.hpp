#pragma once

#include "entity.hpp"
#include "shaderman.hpp"

using namespace ShaderManagement;

class Light : public SimObject::Entity {
public:
    Light(ShaderProgram &sp);

    ShaderProgram lightShader;
    GLuint VAO;
    GLuint VBO;
};
