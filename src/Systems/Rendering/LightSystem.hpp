#pragma once

#include <Components/Primitives/Transform.hpp>
#include <Components/Rendering/PointLight.hpp>
#include <Components/Rendering/DirectionalLight.hpp>
#include <entt/entt.hpp>

namespace LightSystem {
    struct PointLightsHandle {
        std::vector<entt::entity> entities  = {};
    };
    struct DirectionalLightsHandle {
        std::vector<entt::entity> entities      = {};
    };

    void AddPointLight(entt::registry &registry, Transform transform);
    void RenderPointLights(entt::registry &registry);
    void AddDirectionalLight(entt::registry &registry, Transform transform);
    void RenderDirectionalLights(entt::registry &registry);

}
