#pragma once

#include <entt/entt.hpp>

namespace SSBOManager {
    void AddAndInit(entt::registry &registry, std::string ssboName, uint ssboIndex);
};
