
#include "EventSystem.hpp"

namespace EventSystem {

    void PushToUpdateQueue(entt::registry& registry, std::function<void ()> fn){
        auto &queue = registry.ctx().get<UpdateQueue>().functions;
        queue.emplace_back(std::move(fn));
    }

}
