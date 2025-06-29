#include "TimeSystem.hpp"

void CalculateDeltaTime(entt::registry &registry){
    const float currentTime = glfwGetTime();

    auto &[deltaTime, lastTime] = registry.ctx().get<Time>();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
}
