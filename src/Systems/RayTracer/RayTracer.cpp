//
// Created by kshitij on 2/7/25.
//

#include "RayTracer.hpp"

#include <Components/Constants.hpp>
#include <Components/Rendering/Mesh.hpp>
#include <Components/Rendering/Shader.hpp>
#include <Systems/Rendering/ShaderSystem.hpp>

#include "glad/glad.h"

namespace RayTracer {
    void Init(entt::registry &registry, entt::entity &entity) {
        GLuint quadVAO, quadVBO;
        const float quadVertices[] = {
            // positions (NDC)
            -1.0f, -1.0f,
             1.0f, -1.0f,
             1.0f,  1.0f,

            -1.0f, -1.0f,
             1.0f,  1.0f,
            -1.0f,  1.0f
        };

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);

        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        registry.emplace<Mesh>(entity, Mesh{
            .VAO = quadVAO,
            .VBO = quadVBO,
            .initialized = true
        });
    }

    void Render(entt::registry &registry, entt::entity &entity) {
        auto &constants = registry.ctx().get<Constants>();
        auto quadMesh = registry.get<Mesh>(entity);

        auto &shaderEntity = registry.ctx().get<ShaderStore>().shaders["RayTracingShader"];
        auto shader = registry.get<Shader>(shaderEntity);

        glViewport(0, 0, constants.WINDOW_WIDTH, constants.WINDOW_HEIGHT); // match framebuffer size
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        auto clearColor = constants.BACKGROUND_COLOR;
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

        ShaderSystem::Use(shader);
        glBindVertexArray(quadMesh.VAO); // VAO for fullscreen quad
        glDrawArrays(GL_TRIANGLES, 0, 6); // assuming 2 triangles = quad

        glBindVertexArray(0);
        glUseProgram(0);
    }


}