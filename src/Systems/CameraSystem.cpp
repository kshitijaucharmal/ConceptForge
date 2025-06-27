
#include "CameraSystem.hpp"
#include "Components/Rendering/Shader.hpp"
#include "Systems/Rendering/ShaderSystem.hpp"

namespace CameraSystem {


entt::entity CreateCamera(entt::registry &registry, std::string name="Camera"){
    auto camera = registry.create();
    registry.emplace<Transform>(camera, Transform{
        .name = name,
        .position = glm::vec3(7.3589f, 7.3444f, 6.9258f),
        .rotation = glm::vec3(-28.2f, -132.4f, 0.0f) // Pitch (X), Yaw (Y), Roll (Z)
    });
    registry.emplace<Camera>(camera, Camera{
        .MovementSpeed = 3.0f,
        .Fov = 55.0f,
        .Zoom = 55.0f
    });

    auto& camTransform = registry.get<Transform>(camera);
    glm::vec3 origin = glm::vec3(0.0f);
    glm::vec3 front  = glm::vec3(0.0f, 0.0f, -1.0f);
    CameraSystem::LookAt(camTransform, origin + front);

    // Only initialize _this_ camera
    CameraSystem::InitCamera(registry, camera);

    return camera;
}

void InitCamera(entt::registry &reg, entt::entity &entity){
    auto cam = reg.get<Camera>(entity);
    auto transform = reg.get<Transform>(entity);
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

void LookAt(Transform &transform, glm::vec3 target) {
    glm::vec3 direction = glm::normalize(target - transform.position);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Create quaternion that rotates Z to look at direction
    transform.rotation = glm::quatLookAt(direction, up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void ProcessKeyboard(Camera &cam, Transform &transform, CameraMovement direction, float deltaTime) {
    float velocity = cam.MovementSpeed * deltaTime;

    UpdateCameraVectors(cam, transform); // Ensure directions are up-to-date

    if (direction == FORWARD)
        transform.position += cam.Front * velocity;
    if (direction == BACKWARD)
        transform.position -= cam.Front * velocity;
    if (direction == LEFT)
        transform.position -= cam.Right * velocity;
    if (direction == RIGHT)
        transform.position += cam.Right * velocity;
}


// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void ProcessMouseMovement(Camera &cam, Transform &transform, float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= cam.MouseSensitivity;
    yoffset *= cam.MouseSensitivity;

    // Create pitch and yaw quaternions
    glm::quat yawQuat   = glm::angleAxis(glm::radians(-xoffset), glm::vec3(0, 1, 0));
    glm::quat pitchQuat = glm::angleAxis(glm::radians(-yoffset), glm::vec3(1, 0, 0));

    // Apply relative rotation: note order matters
    transform.rotation = glm::normalize(yawQuat * transform.rotation);   // Yaw: around world Y
    transform.rotation = glm::normalize(transform.rotation * pitchQuat); // Pitch: around local X

    // Optionally clamp pitch (a bit tricky with quaternions, skip unless needed)
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void ProcessMouseScroll(Camera &cam, float yoffset)
{
    cam.Zoom -= (float)yoffset;
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
    auto constants = registry.ctx().get<Constants>();

    // Get Active Camera Entity
    auto camEntity = registry.ctx().get<ActiveCamera>().camera;
    auto& camera = registry.get<Camera>(camEntity);
    auto& transform = registry.get<Transform>(camEntity);

    camera.projection = glm::perspective(glm::radians(camera.Fov), constants.ASPECT_RATIO, 0.01f, 100.0f);
    camera.view = GetViewMatrix(camera, transform);

    auto shaderView = registry.view<Shader>();
    for (auto entity : shaderView) {
        auto &shader = shaderView.get<Shader>(entity);
        ShaderSystem::Use(shader);
        ShaderSystem::setMat4(shader, "projection", camera.projection);
        ShaderSystem::setMat4(shader, "view", camera.view);
    }
}


}
