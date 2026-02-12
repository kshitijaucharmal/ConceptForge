
#include "EventSystem.hpp"

namespace EventSystem {

    void PushToUpdateQueue(entt::registry& registry, std::function<void ()> fn){
        auto &queue = registry.ctx().get<UpdateQueue>().functions;
        queue.emplace_back(std::move(fn));
    }
    void PushToLateUpdateQueue(entt::registry& registry, std::function<void ()> fn){
        auto &queue = registry.ctx().get<LateUpdateQueue>().functions;
        queue.emplace_back(std::move(fn));
    }
    void PushToAwakeQueue(entt::registry& registry, std::function<void ()> fn){
        auto &queue = registry.ctx().get<AwakeQueue>().functions;
        queue.emplace_back(std::move(fn));
    }

}
