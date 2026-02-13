
#pragma once

#include <entt/entt.hpp>

namespace Inspector {
    void Show(entt::registry &registry, const entt::entity &selectedObject);
    void Hide(entt::registry &registry);

    // Seperate Windows for all Components
    void ShowDebugInfo();
    void ShowTransform(entt::registry &registry, const entt::entity &selectedObject);
    void ShowDirectionalLight(entt::registry &registry, const entt::entity &selectedObject);
    void ShowPointLight(entt::registry &registry, const entt::entity &selectedObject);
    void ShowCamera(entt::registry &registry, const entt::entity &selectedObject);
}
