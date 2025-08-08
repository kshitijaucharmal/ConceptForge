#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <entt/entt.hpp>
#include <Components/Constants.hpp>
#include <Components/Camera.hpp>
#include <Components/Primitives/Transform.hpp>

// For single camera (takes a reference)
namespace CameraSystem {
    entt::entity CreateCamera(entt::registry &registry, std::string name = "Camera");
    void InitCamera(entt::registry &reg, entt::entity &entity);

    void UpdateCameraVectors(Camera &cam, const Transform &transform);
    glm::mat4 GetViewMatrix(Camera &cam, Transform &transform);

    void SetTransform(Transform &transform, glm::vec3 position, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
    void LookAt(Transform &transform, glm::vec3 target);

    void ProcessKeyboard(Camera &cam, Transform &transform, glm::vec3 direction, float deltaTime);
    void ProcessMouseMovement(Camera &cam, Transform &transform, float xoffset, float yoffset, bool constrainPitch);
    void ProcessMouseScroll(Camera &cam, float yoffset);

    // Projection (Used to be its own class)
    // Calculate for ALL Cameras
    void CalculateProjection(entt::registry &registry);
    void SetView(entt::registry &registry);
}
