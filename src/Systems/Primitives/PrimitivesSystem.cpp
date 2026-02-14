#include "PrimitivesSystem.hpp"

#include <iostream>

#include "Components/Rendering/Material.hpp"
#include "Components/Rendering/Mesh.hpp"

#include "VertexData.hpp"
#include "Components/Primitives/Cube.hpp"
#include "Components/Primitives/UVSphere.hpp"
#include "Core/Physics/PhysicsSystem.hpp"

namespace Primitives {
    entt::entity Create(entt::registry &registry, PrimitiveType primitiveType, Transform transform, entt::entity &material) {
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

    Mesh CreateCubeMesh() {
        GLuint VAO, VBO, EBO;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

        // ADD EBO for indices
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

        // Match shader locations EXACTLY: pos(0), normal(1), tex(2)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);                  // loc0: pos ✓
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))); // loc1: normal (offsets 3+2=5) ✓
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // loc2: texcoord (after pos) ✓
        glEnableVertexAttribArray(2);

        // CRITICAL: Unbind everything
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return Mesh{
            .VAO = VAO, .VBO = VBO, .EBO = EBO,  // Add EBO field
            .indexCount = 36,
            .initialized = true
        };
    }

    entt::entity CreateUVSphereObject(entt::registry &registry, const Transform& transform, const entt::entity &shader, const bool movable) {
        const entt::entity e = registry.create();
        registry.emplace<Transform>(e, transform);
        registry.emplace<Material>(e, Material {
            .shader = shader,
            .initialized = true
        });

        const Mesh mesh = CreateUVSphereMesh();
        registry.emplace<Mesh>(e, mesh);
        registry.emplace<UVSphere>(e);
        registry.emplace<PrimitiveType>(e, UV_SPHERE);

        // Add Rigidbody
        const auto mass = movable ? 1.0f : 0.0f;
        BulletPhysicsSystem::AddRigidbody(e, registry, mass, UV_SPHERE); // mass = 1 -> dynamic

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

    Mesh CreateUVSphereMesh() {
        std::vector<unsigned int> indices;
        std::vector<float> vertices;

        // Default UVSphere
        constexpr auto sphere = UVSphere{};

        GenerateSphereVertices(sphere, vertices);
        GenerateSphereIndices(sphere, indices);

        GLuint VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        constexpr GLsizei stride = 8 * sizeof(float);  // pos(3)+normal(3)+tex(2)

        // Position: layout(location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
        glEnableVertexAttribArray(0);

        // Normal: layout(location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // TexCoord: layout(location = 2)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        const int indicesSize = indices.size();

        return Mesh{
            .VAO = VAO,
            .VBO = VBO,
            .EBO = EBO,
            .indexCount = indicesSize,
            .elemental = true,
            .initialized = true
        };
    }

    void GenerateSphereVertices(const UVSphere &sphere, std::vector<float> &vertices) {
        const float sectorStep = M_PI * 2 / sphere.sectorCount;
        const float stackStep = M_PI / sphere.stackCount;

        for (int i = 0; i <= sphere.stackCount; ++i) {
            const float stackAngle = M_PI / 2 - i * stackStep; // from pi/2 to -pi/2
            const float xy = sphere.radius * cosf(stackAngle);
            float y = sphere.radius * sinf(stackAngle);

            for (int j = 0; j <= sphere.sectorCount; ++j) {
                const float sectorAngle = j * sectorStep;

                float x = xy * cosf(sectorAngle);
                float z = xy * sinf(sectorAngle);

                // normalized normal
                glm::vec3 norm = glm::normalize(glm::vec3(x, y, z));

                // texture coords (u, v)
                float u = static_cast<float>(j) / sphere.sectorCount;
                float v = static_cast<float>(i) / sphere.stackCount;

                // Push in position, texcoord, normal
                vertices.insert(vertices.end(), {x, y, z, u, v, norm.x, norm.y, norm.z});
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
        Mesh mesh = CreateCubeMesh();
        registry.emplace<Mesh>(e, mesh);
        registry.emplace<Cube>(e);
        registry.emplace<PrimitiveType>(e, PrimitiveType::CUBE);

        // Add Rigidbody
        const auto mass = movable ? 1.0f : 0.0f;
        BulletPhysicsSystem::AddRigidbody(e, registry, mass, PrimitiveType::CUBE); // mass = 1 -> dynamic

        return e;
    }
}
