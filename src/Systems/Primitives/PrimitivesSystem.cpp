#include "PrimitivesSystem.hpp"

#include <iostream>

#include "Components/Rendering/Material.hpp"
#include "Components/Rendering/Mesh.hpp"

#include "VertexData.hpp"
#include "Components/SceneRoot.hpp"
#include "Components/Primitives/Cube.hpp"
#include "Components/Primitives/UVSphere.hpp"
#include "Core/Physics/PhysicsSystem.hpp"
#include "Systems/Rendering/MaterialSystem.hpp"
#include "Systems/Rendering/MeshSystem.hpp"

namespace Primitives {
    entt::entity Create(entt::registry &registry, const PrimitiveType primitiveType, const Transform& transform, const entt::entity &material) {
        switch (primitiveType) {
            case PrimitiveType::CUBE:
                return CreateCubeObject(registry, transform, material);
                break;
            case PrimitiveType::UV_SPHERE:
                return CreateUVSphereObject(registry, transform, material);
                break;
        }
        return entt::null;
    }

    Mesh CreateCubeMesh(entt::registry &registry) {
        std::vector<Texture> textures;
        GenerateDefaultTextures(registry, textures);

        return MeshManager::InitMesh(cubeVertices, cubeIndices, textures);
    }

    entt::entity CreateUVSphereObject(entt::registry &registry, const Transform& transform, const entt::entity &shader, const bool movable) {
        const entt::entity e = registry.create();
        registry.emplace<Transform>(e, transform);
        registry.emplace<Material>(e, Material {
            .shader = shader,
            .initialized = true
        });

        const Mesh mesh = CreateUVSphereMesh(registry);
        std::vector meshes = {CreateUVSphereMesh(registry)};
        registry.emplace<std::vector<Mesh>>(e, meshes);
        registry.emplace<UVSphere>(e);
        registry.emplace<PrimitiveType>(e, UV_SPHERE);

        // Add Rigidbody
        const auto mass = movable ? 1.0f : 0.0f;
        BulletPhysicsSystem::AddRigidbody(e, registry, mass, UV_SPHERE); // mass = 1 -> dynamic

        Transform::Reparent(registry, registry.ctx().get<SceneRoot>().entity, e);

        return e;
    }

    void GenerateSphereIndices(const UVSphere &sphere, std::vector<unsigned int> &indices){
        int k1, k2;
        for (int i = 0; i < sphere.stackCount; ++i) {
            k1 = i * (sphere.sectorCount + 1);     // beginning of current stack
            k2 = k1 + sphere.sectorCount + 1;      // beginning of next stack

            for (int j = 0; j < sphere.sectorCount; ++j, ++k1, ++k2) {
                if (i != 0) {
                    // First triangle (k1 -> k2 -> k1+1)
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (sphere.stackCount - 1)) {
                    // Second triangle (k1+1 -> k2 -> k2+1)
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
    }

    void GenerateDefaultTextures(entt::registry &registry, std::vector<Texture> &textures)
    {
        // Just use the White fallback texture for now
        const auto fallback = registry.ctx().get<MaterialSystem::FallbackTexture>();
        textures = {
            Texture{
                .id = fallback,
                .type = "texture_diffuse",
                .path = "fallback"
            },
            Texture{
                .id = fallback,
                .type = "texture_specular",
                .path = "fallback"
            }
        };
    }

    Mesh CreateUVSphereMesh(entt::registry &registry) {
        // Default UVSphere
        constexpr auto sphere = UVSphere{};

        std::vector<unsigned int> indices;
        std::vector<Vertex> vertices;
        std::vector<Texture> textures;

        GenerateSphereVertices(sphere, vertices);
        GenerateSphereIndices(sphere, indices);
        GenerateDefaultTextures(registry, textures);

        return MeshManager::InitMesh(vertices, indices, textures);
    }

    void GenerateSphereVertices(const UVSphere &sphere, std::vector<Vertex> &vertices) {
        vertices.clear();
        vertices.reserve((sphere.stackCount + 1) * (sphere.sectorCount + 1));

        const float sectorStep = 2.0f * M_PI / sphere.sectorCount;
        const float stackStep = M_PI / sphere.stackCount;

        for (int i = 0; i <= sphere.stackCount; ++i) {
            // stackAngle from pi/2 to -pi/2
            const float stackAngle = M_PI / 2.0f - i * stackStep;
            const float xy = sphere.radius * cosf(stackAngle);
            const float y = sphere.radius * sinf(stackAngle);

            for (int j = 0; j <= sphere.sectorCount; ++j) {
                const float sectorAngle = j * sectorStep;

                float x = xy * cosf(sectorAngle);
                float z = xy * sinf(sectorAngle);

                // Normalized normal (for a sphere centered at 0,0,0, this is just pos/radius)
                glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));

                // Texture coords (u, v)
                float u = static_cast<float>(j) / sphere.sectorCount;
                float v = static_cast<float>(i) / sphere.stackCount;

                // Push into the Vertex vector matching your struct layout:
                // struct Vertex { glm::vec3 Position; glm::vec3 Normal; glm::vec2 TexCoords; };
                vertices.emplace_back(Vertex{
                    .Position  = {x, y, z},
                    .Normal    = normal,
                    .TexCoords = {u, v}
                });
            }
        }
    }

    // Creation
    entt::entity CreateCubeObject(entt::registry& registry, const Transform& transform, const entt::entity &shader, const bool movable) {
        const entt::entity e = registry.create();
        registry.emplace<Transform>(e, transform);
        registry.emplace<Material>(e, Material {
            .shader = shader,
            .initialized = true
        });
        std::vector meshes = {CreateCubeMesh(registry)};
        registry.emplace<std::vector<Mesh>>(e, meshes);
        registry.emplace<Cube>(e);
        registry.emplace<PrimitiveType>(e, PrimitiveType::CUBE);

        // Add Rigidbody
        const auto mass = movable ? 1.0f : 0.0f;
        BulletPhysicsSystem::AddRigidbody(e, registry, mass, PrimitiveType::CUBE); // mass = 1 -> dynamic

        Transform::Reparent(registry, registry.ctx().get<SceneRoot>().entity, e);

        return e;
    }
}
