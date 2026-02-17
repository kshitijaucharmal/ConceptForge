#pragma once

#include <Components/Rendering/Mesh.hpp>
#include <glad/glad.h>
#include <entt/entt.hpp>

#include "../../Components/Primitives/UVSphere.hpp"
#include "Components/Primitives/Transform.hpp"

namespace Primitives {
    enum PrimitiveType {
        CUBE,
        UV_SPHERE
    };

    entt::entity Create(entt::registry &registry, PrimitiveType primitiveType, const Transform& transform, const entt::entity &material);

    // Cube
    entt::entity CreateCubeObject(entt::registry& registry, const Transform& transform, const entt::entity &shader, bool movable=true);
    Mesh CreateCubeMesh(entt::registry &registry);

    // Sphere
    entt::entity CreateUVSphereObject(entt::registry& registry, const Transform& transform, const entt::entity &shader, bool movable=true);
    Mesh CreateUVSphereMesh(entt::registry &registry);
    void GenerateSphereVertices(const UVSphere &sphere, std::vector<Vertex> &vertices);
    void GenerateSphereIndices(const UVSphere &sphere, std::vector<unsigned int> &indices);

    void GenerateDefaultTextures(entt::registry &registry, std::vector<Texture> &textures);
}
