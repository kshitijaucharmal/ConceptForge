//
// Created by kshitij on 2/7/25.
//
#pragma once
#include <entt/entt.hpp>

namespace RayTracer {
    void Init(entt::registry &registry, entt::entity &entity);
    void Render(entt::registry &registry, entt::entity &entity);
}
