#pragma once

#include <entt/entt.hpp>
#include "glm/glm.hpp"

enum CameraMovement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

struct Camera {
    // Camera behavior settings
    float MovementSpeed    = 4.5f;
    float MouseSensitivity = 0.1f;

    // Projection parameters
    float Fov              = 45.0f;
    float Zoom             = 1.0f;

    float lastX             = 400.0f;  // Center of screen (change to your window size / 2)
    float lastY             = 300.0f;
    bool firstMouse         = true;

    // Cached direction vectors (computed per frame from Transform)
    glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up    = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);

    // Projection & view matrices (computed per frame)
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view       = glm::mat4(1.0f);

    bool initialized = false;

    // Ray Tracing Params
    float FocalPoint = 1.0f;
    float nearClipPlane = 0.1f;
    float farClipPlane = 100.0f;
};


struct ActiveCamera {
    entt::entity camera;
};
