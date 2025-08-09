#include "PrimitivesSystem.hpp"

#include "Components/Rendering/Material.hpp"
#include "Components/Rendering/Mesh.hpp"
#include "Components/Rendering/MeshFilter.hpp"
#include "Components/Rendering/MeshRenderer.hpp"

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

    entt::entity CreateCubeMesh(entt::registry &registry) {
        GLuint VAO, VBO;
        // Initialize Vertex Array Object (Stores Vertex attribute pointers)
        glGenVertexArrays(1, &VAO);
        // Bind VAO
        glBindVertexArray(VAO);

        // Initialize Vertex Buffer Object
        glGenBuffers(1, &VBO);
        // bind VBO to the buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

        // stride = 8 floats = 8 * sizeof(float)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void *>(nullptr));                     // aPos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));   // aTexCoord
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(5 * sizeof(float)));   // aNormal
        glEnableVertexAttribArray(2);

        const auto cubeMesh = registry.create();
        registry.emplace<Mesh>(cubeMesh, Mesh{
            .VAO = VAO,
            .VBO = VBO,
            .indexCount = 36,
            .initialized = true
        });

        return cubeMesh;
    }

    entt::entity CreateUVSphereObject(entt::registry &registry, Transform transform, entt::entity &shader, bool movable) {
        const entt::entity e = registry.create();
        registry.emplace<Transform>(e, transform);
        registry.emplace<Material>(e, Material {
            .shader = shader,
            .initialized = true
        });

        const entt::entity mesh = CreateUVSphereMesh(registry);
        registry.emplace<MeshFilter>(e, mesh);
        registry.emplace<MeshRenderer>(e);
        // Default
        registry.emplace<UVSphere>(e);
        registry.emplace<PrimitiveType>(e, PrimitiveType::UV_SPHERE);

        // Add Rigidbody
        const auto mass = movable ? 1.0f : 0.0f;
        BulletPhysicsSystem::AddRigidbody(e, registry, mass, PrimitiveType::UV_SPHERE); // mass = 1 -> dynamic

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

    entt::entity CreateUVSphereMesh(entt::registry &registry) {
        std::vector<unsigned int> indices;
        std::vector<float> vertices;

        // Default UVSphere
        auto sphere = UVSphere{};

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

        constexpr GLsizei stride = 8 * sizeof(float);

        // Position: layout(location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, static_cast<void *>(nullptr));
        glEnableVertexAttribArray(0);

        // Texture coordinates: layout(location = 1)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Normals: layout(location = 2)
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        const int indicesSize = indices.size();

        const auto uvSphereMesh = registry.create();
        registry.emplace<Mesh>(uvSphereMesh, Mesh{
            .VAO = VAO,
            .VBO = VBO,
            .EBO = EBO,
            .indexCount = indicesSize,
            .elemental = true,
            .initialized = true
        });

        return uvSphereMesh;
    }

    void GenerateSphereVertices(const UVSphere &sphere, std::vector<float> &vertices) {
        float sectorStep = 2 * M_PI / sphere.sectorCount;
        float stackStep = M_PI / sphere.stackCount;

        for (int i = 0; i <= sphere.stackCount; ++i) {
            float stackAngle = M_PI / 2 - i * stackStep; // from pi/2 to -pi/2
            float xy = sphere.radius * cosf(stackAngle);
            float y = sphere.radius * sinf(stackAngle);

            for (int j = 0; j <= sphere.sectorCount; ++j) {
                float sectorAngle = j * sectorStep;

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
    entt::entity CreateCubeObject(entt::registry& registry, Transform transform, entt::entity &shader, bool movable) {
        const entt::entity e = registry.create();
        registry.emplace<Transform>(e, transform);
        registry.emplace<Material>(e, Material {
            .shader = shader,
            .initialized = true
        });
        const entt::entity mesh = CreateCubeMesh(registry);
        registry.emplace<MeshFilter>(e, mesh);
        registry.emplace<MeshRenderer>(e);
        registry.emplace<Cube>(e);
        registry.emplace<PrimitiveType>(e, PrimitiveType::CUBE);

        // Add Rigidbody
        const auto mass = movable ? 1.0f : 0.0f;
        BulletPhysicsSystem::AddRigidbody(e, registry, mass, PrimitiveType::UV_SPHERE); // mass = 1 -> dynamic

        return e;
    }
}
