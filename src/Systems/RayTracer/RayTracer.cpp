//
// Created by kshitij on 2/7/25.
//

#include "RayTracer.hpp"

#include <Components/Camera.hpp>
#include <Components/Constants.hpp>
#include <Components/SSBOHolder.hpp>
#include <Components/Primitives/Transform.hpp>
#include <Components/Rendering/Mesh.hpp>
#include <Components/Rendering/Shader.hpp>
#include <Core/SSBOManager.hpp>
#include <Systems/Rendering/ShaderSystem.hpp>

#include "Components/RayTracer/RayTracerSettings.hpp"
#include "glad/glad.h"

using namespace Vector3;

namespace RayTracer {
    void Init(entt::registry &registry, entt::entity &entity) {

        InitQuadMesh(registry, entity);
        InitViewport(registry);

    }

    void InitQuadMesh(entt::registry &registry, entt::entity &entity) {
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

    void InitViewport(entt::registry &registry) {
        auto &rtSettings = registry.ctx().get<RayTracerSettings>();
        const auto &cameraEntity = registry.ctx().get<ActiveCamera>().camera;
        const auto &camera = registry.get<Camera>(cameraEntity);
        const auto &cameraTransform = registry.get<Transform>(cameraEntity);
        const auto &constants = registry.ctx().get<Constants>();

        rtSettings.ImageWidth = constants.WINDOW_WIDTH;
        rtSettings.ImageHeight = static_cast<int>(rtSettings.ImageWidth / constants.ASPECT_RATIO);
        rtSettings.ImageHeight = (rtSettings.ImageHeight < 1) ? 1 : rtSettings.ImageHeight;

        rtSettings.invProjection = glm::inverse(camera.projection);
        rtSettings.invView = glm::inverse(camera.view);

        constexpr auto ViewportHeight = 2.0f;
        const auto ViewportWidth = ViewportHeight * (rtSettings.ImageWidth/rtSettings.ImageHeight);

        const auto ViewportU = Vec3(ViewportWidth, 0.0f, 0.0f);
        const auto ViewportV = Vec3(0.0f, ViewportHeight, 0.0f);

        rtSettings.PixelDeltaU = ViewportU / rtSettings.ImageWidth;
        rtSettings.PixelDeltaV = ViewportV / rtSettings.ImageHeight;

        rtSettings.CameraPosition = cameraTransform.position;
        const auto ViewportUpperLeft = rtSettings.CameraPosition - Vec3(0, 0, camera.FocalPoint) - (ViewportU / 2.) - (ViewportV /2.);
        rtSettings.PixelLoc00 = ViewportUpperLeft + 0.5 * (rtSettings.PixelDeltaU + rtSettings.PixelDeltaV);
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

        auto &rtSettings = registry.ctx().get<RayTracerSettings>();
        auto &ssbo = registry.ctx().get<SSBOHolder>().ssbos["rtSettings"];

        // Update values
        auto &activeCamera = registry.ctx().get<ActiveCamera>().camera;
        auto &camera = registry.get<Camera>(activeCamera);
        auto &transform = registry.get<Transform>(activeCamera);

        rtSettings.CameraPosition = transform.position;
        rtSettings.invProjection = glm::inverse(camera.projection);
        rtSettings.invView = glm::inverse(camera.view);

        SSBOManager::UploadToSSBO(ssbo, rtSettings);
    }


}