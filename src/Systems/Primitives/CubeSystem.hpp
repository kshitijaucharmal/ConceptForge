#pragma once

#include <glad/glad.h>
#include <entt/entt.hpp>

#include "Components/Primitives/Transform.hpp"

namespace CubeSystem {
    // Tag for Cube
    struct Cube {};

    entt::entity CreateCubeMesh(entt::registry &registry);
    entt::entity CreateCubeObject(entt::registry& registry, Transform transform, entt::entity &material);

}
