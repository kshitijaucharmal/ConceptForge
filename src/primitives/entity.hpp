#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"

// For GUI
#include "imgui.h"

namespace SimObject {
    class Entity {
    public:
        virtual ~Entity() = default;
        // Transform Components ------------------------------------------
        glm::vec3 position = glm::vec3(0.0);
        glm::vec3 rotation = glm::vec3(0.0);
        glm::vec3 scale = glm::vec3(1.0);

        glm::mat4 model = glm::mat4(1.0f);
        // ---------------------------------------------------------------

        // Transform Functions ---------------------------------------------------------------
        virtual void UpdateModelMatrix();
        virtual void Translate(glm::vec3 pos);
        virtual void Rotate(glm::vec3 rot);
        virtual void Rotate(float angle, glm::vec3 axis);
        virtual void Scale(glm::vec3 factor);
        // -------------------------------------------------------------------------

        virtual void Draw();
        virtual void GUI();
    };
}
