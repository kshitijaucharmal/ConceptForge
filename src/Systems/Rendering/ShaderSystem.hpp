#pragma once

#include <entt/entt.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "Components/Rendering/Shader.hpp"

namespace ShaderSystem {
    // For ALL shaders (Wireframe or filled)
    void InitShaders(entt::registry &registry);
    void SetDrawMode(entt::registry &registry, DrawMode mode);

    // For a single Shader
    void InitVertexShader(Shader &shader, std::string vertexShaderPath);
    void InitFragmentShader(Shader &shader, std::string fragmentShaderPath);
    void LinkShader(Shader& shader);
    void Use(Shader &shader);

    // Setters
    void setBool(Shader &shader, const std::string &name, bool value);
    void setInt(Shader &shader, const std::string &name, int value);
    void setFloat(Shader &shader, const std::string &name, float value);

    // Vec2 overloads
    void setVec2(Shader &shader, const std::string &name, glm::vec2 value);
    void setVec2(Shader &shader, const std::string &name, float x, float y);
    void setVec2(Shader &shader, const std::string &name, float a);

    // vec3 overloads
    void setVec3(Shader &shader, const std::string &name, glm::vec3 value);
    void setVec3(Shader &shader, const std::string &name, float x, float y, float z);
    void setVec3(Shader &shader, const std::string &name, float a);

    // vec4 overloads
    void setVec4(Shader &shader, const std::string &name, glm::vec4 value);
    void setVec4(Shader &shader, const std::string &name, float x, float y, float z, float w);
    void setVec4(Shader &shader, const std::string &name, float a);

    void setMat4(Shader &shader, const std::string &name, glm::mat4 value);

}
