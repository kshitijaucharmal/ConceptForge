
#pragma once

#include <entt/entt.hpp>

namespace Inspector {
    void Show(entt::registry &registry, const entt::entity &selectedObject);
    void Hide(entt::registry &registry);
}
