//
// Created by kshitij on 2/21/26.
//

#include "PickingPassSystem.hpp"

#include <iostream>

#include "ShaderSystem.hpp"
#include "Components/Camera.hpp"
#include "Components/SceneRoot.hpp"
#include "Components/Primitives/Transform.hpp"
#include "Components/Rendering/Material.hpp"
#include "Components/Rendering/Mesh.hpp"
#include "Components/Rendering/PickingBuffer.hpp"
#include "Components/Rendering/Shader.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Systems/SimObjectSystem.hpp"

namespace PickingPassSystem {
    void Init(entt::registry& registry) {

        const auto constants = registry.ctx().get<Constants>();

        GLuint framebufferID, colorTexture, depthBuffer;

        const float width = constants.WINDOW_WIDTH;
        const float height = constants.WINDOW_HEIGHT;

        glGenFramebuffers(1, &framebufferID);
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

        // Create color texture
        glGenTextures(1, &colorTexture);
        glBindTexture(GL_TEXTURE_2D, colorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

        // Check completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

        // Setup FrameBuffer
        auto &framebuffer = registry.ctx().get<PickingBuffer>();
        framebuffer.frameBufferID = framebufferID;
        framebuffer.colorTexture = colorTexture;
        framebuffer.depthBuffer = depthBuffer;

        // unbind
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void BindFramebuffer(entt::registry& registry) {
        const auto &constants = registry.ctx().get<Constants>();
        const auto &framebuffer = registry.ctx().get<PickingBuffer>();
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.frameBufferID);

        // Clear color
        const auto color = glm::vec4(1.0);
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glViewport(0, 0, constants.WINDOW_WIDTH, constants.WINDOW_HEIGHT);
    }

    void UnbindFramebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Render(entt::registry& registry) {
        SimObject::UpdateHierarchyMatrices(registry, registry.ctx().get<SceneRoot>().entity, glm::mat4(1.0f));

        // Bind Shader
        const auto &shaderEntity = registry.ctx().get<ShaderStore>().shaders["PickingShader"];
        auto &shader = registry.get<Shader>(shaderEntity);

        for (const auto view = registry.view<Transform, std::vector<Mesh>, Material>(); const auto entity : view) {
            const auto &_transform = registry.get<Transform>(entity);
            auto &_meshes = registry.get<std::vector<Mesh>>(entity);
            ShaderSystem::Use(shader);
            ShaderSystem::setMat4(shader, "model", _transform.model);

            uint32_t id = static_cast<uint32_t>(entity);
            int r = (id & 0x000000FF) >>  0;
            int g = (id & 0x0000FF00) >>  8;
            int b = (id & 0x00FF0000) >> 16;

            ShaderSystem::setVec4(shader, "PickingColor", glm::vec4(r/255.0, g/255.0, b/255.0, 1.0f));

            // Texture rendering
            for (const auto &_mesh : _meshes)
            {
                glBindVertexArray(_mesh.VAO);

                // Draw differently depending on EBO availability
                if (_mesh.elemental) glDrawElements(GL_TRIANGLES, _mesh.indexCount, GL_UNSIGNED_INT, nullptr);
                else glDrawArrays(GL_TRIANGLES, 0, _mesh.indexCount);
            }
        }
        glBindVertexArray(0);
    }

}
