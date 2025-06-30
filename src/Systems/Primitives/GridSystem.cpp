//
// Created by kshitij on 28/6/25.
//

#include "GridSystem.hpp"

#include <iostream>
#include <Components/Primitives/Transform.hpp>
#include <Components/Rendering/Material.hpp>

#include "Components/Primitives/Grid.hpp"
#include <glad/glad.h>
#include <Systems/Rendering/ShaderSystem.hpp>

#include "Components/Rendering/MeshFilter.hpp"
#include "Components/Rendering/Mesh.hpp"

namespace GridSystem {
    entt::entity GenerateMesh(entt::registry& registry, const Grid grid) {
        std::vector<float> vertices;

        for (int i = -grid.size; i <= grid.size; ++i) {
            // Z-axis lines (along X)
            vertices.insert(vertices.end(), {
                i * grid.spacing, 0.0f, -grid.size * grid.spacing,
                i * grid.spacing, 0.0f,  grid.size * grid.spacing
            });

            // X-axis lines (along Z)
            vertices.insert(vertices.end(), {
                -grid.size * grid.spacing, 0.0f, i * grid.spacing,
                 grid.size * grid.spacing, 0.0f, i * grid.spacing
            });
        }

        GLuint vao, vbo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0); // assuming layout(location = 0) in shader
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));

        glBindVertexArray(0);

        const auto mesh = registry.create();
        registry.emplace<Mesh>(mesh, vao, vbo, vertices.size() / 3);
        return mesh;
    }

    entt::entity CreateGrid(entt::registry &registry, entt::entity &shader, std::string name) {
        const entt::entity entity = registry.create();
        registry.emplace<Material>(entity, Material{.shader = shader});
        registry.emplace<Grid>(entity, Grid{ });
        auto mesh = GenerateMesh(registry, registry.get<Grid>(entity));
        registry.emplace<MeshFilter>(entity, mesh);
        return entity;
    }

    void Render(entt::registry& registry, const entt::entity gridEntity, Shader &shader) {
        const auto meshEntity = registry.get<MeshFilter>(gridEntity).meshEntity;
        const auto mesh = registry.get<Mesh>(meshEntity);
        const auto grid = registry.get<Grid>(gridEntity);

        const auto cameraEntity = registry.ctx().get<ActiveCamera>().camera;
        const auto camera = registry.get<Camera>(cameraEntity);
        const auto camTransform = registry.get<Transform>(cameraEntity);

        auto MVP = camera.projection * camera.view;
        auto camXZ = glm::vec3(camTransform.position.x, 0.0f, camTransform.position.z);
        auto snappedOffset = glm::floor(camXZ / grid.spacing) * grid.spacing;

        ShaderSystem::Use(shader);
        ShaderSystem::setMat4(shader, "MVP", MVP);
        ShaderSystem::setVec3(shader, "uColor", grid.color);
        ShaderSystem::setVec3(shader, "gridOffset", snappedOffset);

        glBindVertexArray(mesh.VAO);
        glDrawArrays(GL_LINES, 0, mesh.indexCount);
        glBindVertexArray(0);
    }
}
