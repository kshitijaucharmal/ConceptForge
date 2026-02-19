#pragma once

#include <Components/Primitives/Transform.hpp>
#include <Components/Rendering/PointLight.hpp>
#include <Components/Rendering/DirectionalLight.hpp>
#include <entt/entt.hpp>

namespace LightSystem {
    entt::entity AddPointLight(entt::registry &registry, Transform transform, PointLight point_light);
    void RenderPointLights(entt::registry &registry);
    entt::entity AddDirectionalLight(entt::registry &registry, const Transform& transform, DirectionalLight directional_light);
    void RenderDirectionalLights(entt::registry &registry);

}
