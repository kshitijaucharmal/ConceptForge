#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

// GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"

// For GUI
#include "imgui.h"

#include "shaderman.hpp"

namespace SimObject {
    class Entity {
    public:
        virtual ~Entity() = default;
        // Transform Components ------------------------------------------
        glm::vec3 position = glm::vec3(0.0);
        glm::vec3 rotation = glm::vec3(0.0);
        glm::vec3 scale = glm::vec3(1.0);

        glm::mat4 model = glm::mat4(1.0f);
        // TODO Gotta be some kinda id too
        std::string name = "Entity";

        ShaderManagement::ShaderProgram *shaderProgram;
        // ---------------------------------------------------------------

        // Transform Functions ---------------------------------------------------------------
        virtual void UpdateModelMatrix();
        virtual glm::mat4 GetModel();
        virtual void* GetModelPointer();
        // Get
        virtual glm::vec3 GetPosition() const;
        virtual glm::vec3 GetRotation() const;
        virtual glm::vec3 GetScale() const;
        // Set
        virtual void SetPosition(glm::vec3 pos);
        virtual void SetRotation(glm::vec3 rot);
        virtual void SetScale(glm::vec3 scale);

        // Move by
        virtual void Translate(glm::vec3 delta);
        virtual void Rotate(glm::vec3 rot);
        virtual void Rotate(float angle, glm::vec3 axis);
        virtual void Scale(glm::vec3 factorDelta);
        // -------------------------------------------------------------------------

        virtual void Draw();
    };
}
