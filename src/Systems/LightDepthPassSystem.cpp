//
// Created by kshitij on 2/20/26.
//

#include "LightDepthPassSystem.hpp"

#include <iostream>

#include "Components/Constants.hpp"
#include "Components/Primitives/Transform.hpp"
#include "Components/Rendering/DirectionalLight.hpp"
#include "Components/Rendering/LightPassFrameBuffer.hpp"
#include "Components/Rendering/Mesh.hpp"
#include "Components/Rendering/Shader.hpp"
#include "Rendering/LightSystem.hpp"
#include "Rendering/ShaderSystem.hpp"

namespace LightDepthPassSystem
{
    // Only for directional lights for now
    // Setting max lights to 2 (Won't need anymore directional lights)
    constexpr int NUM_LIGHTS = 2;

    void Init(entt::registry& registry)
    {
        const auto &constants = registry.ctx().get<Constants>();

        // Setup FrameBuffer
        auto &framebuffer = registry.ctx().get<LightPassFrameBuffer>();

        // Init as a TEXTURE_2D_ARRAY
        glGenTextures(1, &framebuffer.shadowDepthArray);
        glBindTexture(GL_TEXTURE_2D_ARRAY, framebuffer.shadowDepthArray);

        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT,
            framebuffer.width, framebuffer.height, NUM_LIGHTS, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        // Border prevents bleeding
        constexpr float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);

        // Framebuffer
        // Don't need to add any texture, will be swapping textures
        glGenFramebuffers(1, &framebuffer.frameBufferID);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.frameBufferID);

        // Only depth
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        // unbind
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void BindFramebuffer(entt::registry& registry)
    {
        const auto fb = registry.ctx().get<LightPassFrameBuffer>();
        glEnable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, fb.frameBufferID);
        glViewport(0, 0, fb.width, fb.height);
    }

    void UnbindFramebuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Render(entt::registry& registry){
        const auto lightPassFB = registry.ctx().get<LightPassFrameBuffer>();
        glViewport(0, 0, lightPassFB.width, lightPassFB.height);

        // Render All Shadow Maps (Later can prioritize by distance to player)
        const auto &entities = registry.view<Transform, DirectionalLight>();
        const auto& meshView = registry.view<Transform, std::vector<Mesh>>();

        const auto shadowShaderEntity = registry.ctx().get<ShaderStore>().shaders["ShadowShader"];
        auto &shadowShader = registry.get<Shader>(shadowShaderEntity);
        ShaderSystem::Use(shadowShader);

        // Understand that max is 2 (declared in Init)
        int shadowCounter = 0;
        for (const entt::entity entity : entities) {
            auto& transform = entities.get<Transform>(entity);
            auto& light = entities.get<DirectionalLight>(entity);
            // Set shadowMapIndex
            if (light.castShadows == 1.0) {
                if (shadowCounter >= NUM_LIGHTS) break;
                light.shadowMapIndex = shadowCounter++;
            } else {
                light.shadowMapIndex = -1;
                continue;
            }

            // Attach the correct layer
            glFramebufferTextureLayer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, lightPassFB.shadowDepthArray, 0, light.shadowMapIndex);
            glClear(GL_DEPTH_BUFFER_BIT);

            light.direction = glm::normalize(glm::eulerAngles(transform.rotation));
            light.lightSpaceMatrix = LightSystem::CalculateLightSpaceMatrix(light.direction);

            // Set the light space matrix
            ShaderSystem::setMat4(shadowShader, "lightSpaceMatrix", light.lightSpaceMatrix);

            // Draw all entities' geometry only
            for (const auto& meshEntity : meshView) {
                auto& _transform = meshView.get<Transform>(meshEntity);
                auto& _meshes = meshView.get<std::vector<Mesh>>(meshEntity);

                ShaderSystem::setMat4(shadowShader, "model", _transform.model);

                for (auto& _mesh : _meshes) {
                    glBindVertexArray(_mesh.VAO);

                    if (_mesh.elemental)
                        glDrawElements(GL_TRIANGLES, _mesh.indexCount, GL_UNSIGNED_INT, nullptr);
                    else
                        glDrawArrays(GL_TRIANGLES, 0, _mesh.indexCount);
                }
            }
        }
    }
}
