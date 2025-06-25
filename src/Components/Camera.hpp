#pragma once

#include "glm/glm.hpp"

enum CameraMovement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

struct Camera {
    // camera Attributes
    glm::vec3 Position      = glm::vec3();
    glm::vec3 Front         = glm::vec3();
    glm::vec3 Up            = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Right         = glm::vec3();
    glm::vec3 WorldUp       = glm::vec3(0.0f, 1.0f, 0.0f);

    // euler Angles
    float Yaw               = -90.0f;
    float Pitch             = 0.0f;
    // camera options
    float MovementSpeed     = 2.5f;
    float MouseSensitivity  = 0.1f;
    float Zoom              = 45.0f;
    float Fov               = 45.0f;

    // Projection
    glm::mat4 projection    = glm::mat4(1.0);
    glm::mat4 view          = glm::mat4(1.0);
};
