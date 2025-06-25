#pragma once

#include "entt/entt.hpp"

namespace EventSystem{
    struct UpdateQueue{ std::vector<std::function<void()>> functions; };
    struct AwakeQueue{ std::vector<std::function<void()>> functions; };
    struct LateUpdateQueue{ std::vector<std::function<void()>> functions; };

    void PushToAwakeQueue(entt::registry& registry, std::function<void ()> fn);
    void PushToUpdateQueue(entt::registry& registry, std::function<void ()> fn);
    void PushToLateUpdateQueue(entt::registry& registry, std::function<void ()> fn);
}
