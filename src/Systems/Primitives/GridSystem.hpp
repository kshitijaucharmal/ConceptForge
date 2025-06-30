#pragma once

#include <Components/Camera.hpp>
#include <Components/Rendering/Shader.hpp>
#include <entt/entt.hpp>

#include "Components/Primitives/Grid.hpp"

namespace  GridSystem {
    entt::entity GenerateMesh(entt::registry& registry, const Grid grid);
    entt::entity CreateGrid(entt::registry &registry, entt::entity &shader, std::string name);
    void Render(entt::registry& registry, const entt::entity gridEntity, Shader &shader);
}
