
#include "CameraSystem.hpp"

void InitCamera(entt::registry &reg, entt::entity &entity){
    auto cam = reg.get<Camera>(entity);
    UpdateCameraVectors(cam);
}

// calculates the front vector from the Camera's (updated) Euler Angles
void UpdateCameraVectors(Camera &cam) {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(cam.Yaw)) * cos(glm::radians(cam.Pitch));
    front.y = sin(glm::radians(cam.Pitch));
    front.z = sin(glm::radians(cam.Yaw)) * cos(glm::radians(cam.Pitch));
    cam.Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    cam.Right = glm::normalize(glm::cross(cam.Front, cam.WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    cam.Up    = glm::normalize(glm::cross(cam.Right, cam.Front));
}

void SetTransform(Camera &cam, glm::vec3 position, glm::vec3 up){
    cam.Position = position;
    cam.WorldUp = up;
    UpdateCameraVectors(cam);
}

void LookAt(Camera &cam, glm::vec3 target) {
    // Calculate the new Front vector by subtracting the camera position from the target position
    cam.Front = glm::normalize(target - cam.Position);

    // Recalculate the Right and Up vectors based on the new Front vector
    cam.Right = glm::normalize(glm::cross(cam.Front, cam.WorldUp));  // The Right vector is perpendicular to Front and WorldUp
    cam.Up = glm::normalize(glm::cross(cam.Right, cam.Front));  // The Up vector is perpendicular to both Right and Front

    // Update the view matrix based on the new camera orientation
    UpdateCameraVectors(cam);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void ProcessKeyboard(Camera &cam, CameraMovement direction, float deltaTime)
{
    float velocity = cam.MovementSpeed * deltaTime;
    if (direction == FORWARD)
        cam.Position += cam.Front * velocity;
    if (direction == BACKWARD)
        cam.Position -= cam.Front * velocity;
    if (direction == LEFT)
        cam.Position -= cam.Right * velocity;
    if (direction == RIGHT)
        cam.Position += cam.Right * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void ProcessMouseMovement(Camera &cam, float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= cam.MouseSensitivity;
    yoffset *= cam.MouseSensitivity;

    cam.Yaw   += xoffset;
    cam.Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (cam.Pitch > 89.0f)
            cam.Pitch = 89.0f;
        if (cam.Pitch < -89.0f)
            cam.Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors(cam);
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
glm::mat4 GetViewMatrix(Camera &cam)
{
    return glm::lookAt(cam.Position, cam.Position + cam.Front, cam.Up);
}

void CalculateProjection(entt::registry &registry){
    auto constants = registry.ctx().get<Constants>();

    auto view = registry.view<Camera>();
    for(auto entity : view){
        auto &cam = view.get<Camera>(entity);
        // Doing this in update to update in realtime when Fov changes
        cam.projection = glm::perspective(glm::radians(cam.Fov), constants.ASPECT_RATIO, 0.01f, 100.0f);
        cam.view = GetViewMatrix(cam);
    }
}
