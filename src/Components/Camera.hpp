#pragma once

#include <entt/entt.hpp>
#include "glm/glm.hpp"

/**
 * The Camera struct stores the state, configuration, and
 * computed matrices required for 3D rendering and player interaction.
 */
struct Camera {
    // Camera behavior settings
    /// Defines how many units the camera moves per second.
    float MovementSpeed    = 4.5f;
    /// A multiplier applied to mouse delta inputs to control look speed.
    float MouseSensitivity = 0.1f;

    /// The vertical Field of View in degrees; determines the perspective "width."
    float Fov              = 45.0f;
    /// A scalar value used to modify the field of view or focal length.
    float Zoom             = 1.0f;

    // TODO: Just make it a vector
    /// Previous frame mouse coordinates - 'x'
    float lastX             = 400.0f;  // Center of screen (change to your window size / 2)
    /// Previous frame mouse coordinates - 'y'
    float lastY             = 300.0f;

    /// A flag used to prevent sudden "jumps" when the camera first captures the mouse cursor.
    bool firstMouse         = true;

    // Cached direction vectors (computed per frame from Transform)
    /// The normalized vector pointing in the direction the camera is facing.
    glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
    /// The normalized vector pointing "up" relative to the camera's orientation.
    glm::vec3 Up    = glm::vec3(0.0f, 1.0f, 0.0f);
    /// The normalized vector pointing to the camera's right side (orthogonal to Front and Up).
    glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);

    // Projection & view matrices (computed per frame)
    /// The matrix that transforms 3D view-space coordinates into 2D clip-space (Perspective or Orthographic).
    glm::mat4 projection = glm::mat4(1.0f);
    /// The matrix that transforms world-space coordinates into camera-relative coordinates.
    glm::mat4 view       = glm::mat4(1.0f);

    /// Ensures the camera setup (like window-size dependent values) is performed before the first render pass.
    bool initialized = false;

    // Ray Tracing Params
    /// Used for depth-of-field calculations or ray-target intersections.
    float FocalPoint = 1.0f;
    /// The closest distance at which objects are rendered.
    float nearClipPlane = 0.1f;
    /// The maximum distance at which objects are rendered.
    float farClipPlane = 100.0f;
};

/**
 * The ActiveCamera struct serves as a Singleton Component or a "Tag."
 *
 * Purpose: Allows systems to easily query which camera to use for the main render pass without searching through all entities containing a Camera component.
 */
struct ActiveCamera {
    entt::entity camera;
};
