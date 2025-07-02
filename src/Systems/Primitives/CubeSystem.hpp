#pragma once

#include <glad/glad.h>
#include <entt/entt.hpp>

#include "Components/Primitives/Transform.hpp"

namespace Primitives {
    // Cube
    entt::entity CreateCubeMesh(entt::registry &registry);
    entt::entity CreateCubeObject(entt::registry& registry, Transform transform, entt::entity &material, bool movable=true);

    // Sphere
    entt::entity CreateUVSphereMesh(entt::registry &registry);
    entt::entity CreateUVSphereObject(entt::registry& registry, Transform transform, entt::entity &material, bool movable=true);
    std::vector<float> GenerateSphere(entt::registry& registry);
    std::vector<unsigned int> GenerateSphereIndices(entt::registry &registry);
    void CreateUVSphereVAO(entt::registry& registry, std::vector<float>& vertices, std::vector<unsigned int>& indices);


}
