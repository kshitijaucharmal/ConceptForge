#pragma once

#include "entt/entt.hpp"

/**
 * Event System is for calling a queue of functions at a specific time
 * These will be called at appropriate times in a game
 */
namespace EventSystem{
    /// Update Queue is called every frame
    struct UpdateQueue{ std::vector<std::function<void()>> functions; };
    /// Awake Queue is called at the start of the engine (should be when pressed start)
    struct AwakeQueue{ std::vector<std::function<void()>> functions; };
    /// Called after Update
    struct LateUpdateQueue{ std::vector<std::function<void()>> functions; };

    /// Everything in this is deleted at the end of the frame
    struct DeleteQueue { std::set<entt::entity> entities; };

    void PushToAwakeQueue(entt::registry& registry, std::function<void ()> fn);
    void PushToUpdateQueue(entt::registry& registry, std::function<void ()> fn);
    void PushToLateUpdateQueue(entt::registry& registry, std::function<void ()> fn);
}
