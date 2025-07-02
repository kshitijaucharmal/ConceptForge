#pragma once

#include <glad/glad.h>
#include <entt/entt.hpp>

#include "../../Components/Primitives/UVSphere.hpp"
#include "Components/Primitives/Transform.hpp"

namespace Primitives {
    enum PrimitiveType {
        CUBE,
        UV_SPHERE
    };

    entt::entity Create(entt::registry &registry, PrimitiveType primitiveType, Transform transform, entt::entity &material);

    // Cube
    entt::entity CreateCubeObject(entt::registry& registry, Transform transform, entt::entity &shader, bool movable=true);
    entt::entity CreateCubeMesh(entt::registry &registry);

    // Sphere
    entt::entity CreateUVSphereObject(entt::registry& registry, Transform transform, entt::entity &shader, bool movable=true);
    entt::entity CreateUVSphereMesh(entt::registry &registry);
    void GenerateSphereVertices(const UVSphere &sphere, std::vector<float> &vertices);
    void GenerateSphereIndices(const UVSphere &sphere, std::vector<unsigned int> &indices);
}
