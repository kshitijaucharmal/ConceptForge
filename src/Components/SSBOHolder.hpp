#pragma once

#include <entt/entt.hpp>

struct SSBOHolder {
    std::unordered_map<std::string, unsigned int> ssbos;
    int ssboCount = 0;
};