#include "CubeSystem.hpp"

#include <iostream>

#include "Components/Rendering/Material.hpp"
#include "Components/Rendering/Mesh.hpp"
#include "Components/Rendering/MeshFilter.hpp"
#include "Components/Rendering/MeshRenderer.hpp"

#include "VertexData.hpp"
#include "../../../external/bullet/src/BulletCollision/CollisionShapes/btBoxShape.h"
#include "../../Core/Physics/PhysicsSystem.hpp"

namespace CubeSystem {
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

        // Add Rigidbody
        const auto scale = btVector3(transform.scale.x, transform.scale.y, transform.scale.z);
        const auto halfScale = btVector3(scale.x()/2, scale.y()/2, scale.z()/2);
        auto* shape = new btBoxShape(btVector3(1, 1, 1));
        shape->setLocalScaling(halfScale); // box half extents

        const auto mass = movable ? 1.0f : 0.0f;
        BulletPhysicsSystem::AddRigidbody(e, registry, mass, shape); // mass = 1 -> dynamic

        return e;
    }
}
