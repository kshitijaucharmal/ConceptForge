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
        // Transform Components ------------------------------------------
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        glm::mat4 model = glm::mat4(1.0f);
        // ---------------------------------------------------------------

        // Transform Functions ---------------------------------------------------------------
        virtual void Translate(glm::vec3 pos) = 0;
        virtual void Rotate(glm::vec3 rot) = 0;
        virtual void Rotate(float angle, glm::vec3 axis) = 0;
        virtual void Scale(glm::vec3 factor) = 0;
        virtual void UpdateModelMatrix() = 0;
        // -------------------------------------------------------------------------

        virtual void Draw() = 0;
        virtual void GUI() = 0;
    };
}
