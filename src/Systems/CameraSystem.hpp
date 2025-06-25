#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <entt/entt.hpp>
#include <Components/Constants.hpp>
#include <Components/Camera.hpp>

// For single camera (takes a reference)
void InitCamera(entt::registry &reg, entt::entity &entity);
void UpdateCameraVectors(Camera &cam);
glm::mat4 GetViewMatrix(Camera &cam);
void SetTransform(Camera &cam, glm::vec3 position, glm::vec3 up);
void LookAt(Camera &cam, glm::vec3 target);
void ProcessKeyboard(Camera &cam, CameraMovement direction, float deltaTime);
void ProcessMouseMovement(Camera &cam, float xoffset, float yoffset, bool constrainPitch);
void ProcessMouseScroll(Camera &cam, float yoffset);

// Projection (Used to be its own class)
// Calculate for ALL Cameras
void CalculateProjection(entt::registry &registry);
