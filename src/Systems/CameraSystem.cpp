#include "CameraSystem.hpp"

#include <utility>
#include "Components/Rendering/Shader.hpp"
#include "Systems/Rendering/ShaderSystem.hpp"

namespace CameraSystem {

entt::entity CreateCamera(entt::registry &registry, std::string name="Camera"){
    auto &constants = registry.ctx().get<Constants>();
    auto camera = registry.create();
    registry.emplace<Transform>(camera, Transform{
        .name = std::move(name),
        .position = glm::vec3(0.0, 2.0, 10.0f),
    });
    registry.emplace<Camera>(camera, Camera{
        .Fov = 60.0f,
    });

    // Only initialize _this_ camera
    CameraSystem::InitCamera(registry, camera);

    return camera;
}

void InitCamera(entt::registry &reg, entt::entity &entity){
    auto cam = reg.get<Camera>(entity);
    const auto transform = reg.get<Transform>(entity);
    UpdateCameraVectors(cam, transform);
}

// calculates the front vector from the Camera's (updated) Euler Angles
void UpdateCameraVectors(Camera &cam, const Transform &transform) {
    cam.Front = glm::normalize(transform.rotation * glm::vec3(0, 0, -1));
    cam.Right = glm::normalize(transform.rotation * glm::vec3(1, 0, 0));
    cam.Up    = glm::normalize(transform.rotation * glm::vec3(0, 1, 0));
}

void SetTransform(Transform &transform, glm::vec3 position, glm::vec3 up) {
    transform.position = position;

    // Optional: If you want to align "Up", you can compute rotation accordingly
    // but typically Up is always (0, 1, 0) and handled via cross products.
}

void LookAt(Transform &transform, const glm::vec3 target) {
    const auto direction = glm::normalize(target - transform.position);
    const auto up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Create quaternion that rotates Z to look at direction
    transform.rotation = glm::quatLookAt(direction, up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void ProcessKeyboard(Camera &cam, Transform &transform, glm::vec3 direction, const float deltaTime) {
    const float velocity = cam.MovementSpeed * deltaTime;

    if (glm::length(direction) < 0.01f)
        return;

    direction = glm::normalize(direction);

    UpdateCameraVectors(cam, transform); // Ensure directions are up-to-date
    const auto move = glm::normalize(cam.Right * direction.x + cam.Up * direction.y + cam.Front * direction.z) * velocity;
    transform.position += move;
}


// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void ProcessMouseMovement(Camera &cam, Transform &transform, float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= cam.MouseSensitivity;
    yoffset *= cam.MouseSensitivity;

    // Create pitch and yaw quaternions
    const glm::quat yawQuat   = glm::angleAxis(glm::radians(-xoffset), glm::vec3(0, 1, 0));
    const glm::quat pitchQuat = glm::angleAxis(glm::radians(-yoffset), glm::vec3(1, 0, 0));

    // Apply relative rotation: note order matters
    transform.rotation = glm::normalize(yawQuat * transform.rotation);   // Yaw: around world Y
    transform.rotation = glm::normalize(transform.rotation * pitchQuat); // Pitch: around local X

    // Optionally clamp pitch (a bit tricky with quaternions, skip unless needed)
}

// processes input received from a mouse scroll-wheel event. Only requires to be input on the vertical wheel-axis
void ProcessMouseScroll(Camera &cam, const float yoffset)
{
    cam.Zoom -= static_cast<float>(yoffset);
    if (cam.Zoom < 1.0f)
        cam.Zoom = 1.0f;
    if (cam.Zoom > 45.0f)
        cam.Zoom = 45.0f;
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 GetViewMatrix(Camera &cam, Transform &transform) {
    UpdateCameraVectors(cam, transform);
    return glm::lookAt(transform.position, transform.position + cam.Front, cam.Up);
}

void CalculateProjection(entt::registry &registry){
    const auto constants = registry.ctx().get<Constants>();

    // Get Active Camera Entity
    auto camEntity = registry.ctx().get<ActiveCamera>().camera;
    auto& camera = registry.get<Camera>(camEntity);
    auto& transform = registry.get<Transform>(camEntity);

    camera.projection = glm::perspective(glm::radians(camera.Fov), constants.ASPECT_RATIO, 0.01f, 100.0f);
    camera.view = GetViewMatrix(camera, transform);

    for (const auto shaderView = registry.view<Shader>(); const auto entity : shaderView) {
        auto &shader = shaderView.get<Shader>(entity);
        ShaderSystem::Use(shader);
        ShaderSystem::setMat4(shader, "projection", camera.projection);
        ShaderSystem::setMat4(shader, "view", camera.view);
    }
}

void SetView(entt::registry &registry) {
    auto &shaderStore = registry.ctx().get<ShaderStore>();
    auto &camera = registry.ctx().get<ActiveCamera>().camera;
    auto &camTransform = registry.get<Transform>(camera);
    for (auto &[name, shaderEntity] : shaderStore.shaders) {
        auto &shader = registry.get<Shader>(shaderEntity);
        ShaderSystem::Use(shader);
        ShaderSystem::setVec3(shader, "viewPos", camTransform.position);
        ShaderSystem::setFloat(shader, "material.shininess", 32.0f);
    }
}
}
