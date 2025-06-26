#include "CubeSystem.hpp"

#include "VertexData.hpp"

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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);                     // aPos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));   // aTexCoord
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));   // aNormal
        glEnableVertexAttribArray(2);

        // UpdateModelMatrix not included

        auto cubeMesh = registry.create();
        registry.emplace<Mesh>(cubeMesh, Mesh{
            .VAO = VAO,
            .VBO = VBO,
            .indexCount = 36,
            .initialized = true
        });

        return cubeMesh;
    }

    entt::entity CreateCubeObject(entt::registry& registry, Transform &transform, entt::entity &shader) {
        entt::entity e = registry.create();
        registry.emplace<Transform>(e, transform);
        registry.emplace<Material>(e, Material {
            .shader = shader,
            .initialized = true
        });
        entt::entity mesh = CreateCubeMesh(registry);
        registry.emplace<MeshFilter>(e, MeshFilter{.meshEntity = mesh});
        registry.emplace<MeshRenderer>(e);
        registry.emplace<Cube>(e);
        return e;
    }
}
