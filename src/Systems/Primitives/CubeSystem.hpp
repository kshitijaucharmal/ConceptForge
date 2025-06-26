#pragma once

#include <glad/glad.h>
#include <entt/entt.hpp>

#include <iostream>

#include "Components/Rendering/Material.hpp"
#include "Components/Rendering/Mesh.hpp"
#include "Components/Rendering/MeshFilter.hpp"
#include "Components/Rendering/MeshRenderer.hpp"
#include "Components/Primitives/Transform.hpp"

namespace CubeSystem {
    // Tag for Cube
    struct Cube {};

    entt::entity CreateCubeMesh(entt::registry &registry);
    entt::entity CreateCubeObject(entt::registry& registry, Transform &transform, entt::entity &material);

}
