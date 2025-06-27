#pragma once

#include <entt/entt.hpp>
#include <vector>
#include "imgui.h"

namespace Hierarchy {
    struct Hierarchy {
        // Holds references to each hierarchy item
        std::vector<entt::entity> entities;
        int selectedID = -1;
        entt::entity selectedEntity = entt::null;
    };

    void Show(entt::registry &registry);
}
