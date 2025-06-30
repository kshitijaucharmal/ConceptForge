#pragma once

#include <entt/entt.hpp>

struct SSBOHolder {
    std::unordered_map<std::string, uint> ssbos;
    int ssboCount = 0;
};