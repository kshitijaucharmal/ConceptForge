#include "TimeSystem.hpp"

void CalculateDeltaTime(entt::registry &registry){
    float currentTime = glfwGetTime();

    auto time = registry.ctx().get<Time>();
    time.deltaTime = currentTime - time.lastTime;
    time.lastTime = currentTime;
}
