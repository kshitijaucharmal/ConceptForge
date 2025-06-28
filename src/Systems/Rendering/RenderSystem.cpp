#include "RenderSystem.hpp"

#include "Components/Primitives/Transform.hpp"
#include "Components/Rendering/Material.hpp"
#include "Components/Rendering/Shader.hpp"

#include "Components/Rendering/Mesh.hpp"
#include "Components/Rendering/MeshFilter.hpp"
#include "Components/Rendering/MeshRenderer.hpp"
#include "Components/Rendering/FrameBuffer.hpp"
#include "Components/Constants.hpp"

#include "Systems/SimObjectSystem.hpp"
#include "Systems/Primitives/GizmoSystem.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace RenderSystem {
    void Init(entt::registry &registry){
        auto constants = registry.ctx().get<Constants>();

        int width = constants.WINDOW_WIDTH;
        int height = constants.WINDOW_HEIGHT;

        GLuint framebufferID, colorTexture, depthBuffer;

        glGenFramebuffers(1, &framebufferID);
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

        // Create color texture
        glGenTextures(1, &colorTexture);
        glBindTexture(GL_TEXTURE_2D, colorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

        // Create depth renderbuffer
        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

        // Check completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

        auto &framebuffer = registry.ctx().get<FrameBuffer>();
        framebuffer.colorTexture = colorTexture;
        framebuffer.depthBuffer = depthBuffer;
        framebuffer.frameBufferID = framebufferID;

        // unbind
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void BindFramebuffer(entt::registry &registry) {
        const auto &framebuffer = registry.ctx().get<FrameBuffer>();
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.frameBufferID);
    }

    void UnbindFramebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER,0);
    }

    void Render(entt::registry& registry){
        const auto constants = registry.ctx().get<Constants>();

        glViewport(0, 0, constants.WINDOW_WIDTH, constants.WINDOW_HEIGHT);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(constants.CLEAR_COLOR.r,
                        constants.CLEAR_COLOR.g,
                        constants.CLEAR_COLOR.b,
                        constants.CLEAR_COLOR.a);
        // Clear Color Buffer and Depth Buffer

        for (const auto view = registry.view<Transform, MeshFilter, MeshRenderer, Material>(); const auto entity : view) {
            auto& t = view.get<Transform>(entity);
            auto& f = view.get<MeshFilter>(entity);
            auto& m = view.get<Material>(entity);

            const Shader* shader = registry.try_get<Shader>(m.shader);
            const Mesh* mesh = registry.try_get<Mesh>(f.meshEntity);
            if (!shader || !mesh || !shader->initialized || !mesh->initialized) continue;

            // Replace with helpers
            auto model = SimObject::ComposeTransform(t);
            glUseProgram(shader->shaderID);
            glUniformMatrix4fv(glGetUniformLocation(shader->shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(mesh->VAO);
            glDrawArrays(GL_TRIANGLES, 0, mesh->indexCount);
            glBindVertexArray(0);
        }

    }

    void ShowSceneTexture(entt::registry &registry) {
        auto &constants = registry.ctx().get<Constants>();
        // Fixed Screen Position
        ImGui::SetNextWindowPos(ImVec2(constants.SCENE_X, constants.SCENE_Y));
        ImGui::SetNextWindowSize(ImVec2(constants.SCENE_WIDTH, constants.SCENE_HEIGHT));
        ImGui::Begin("Scene",
                     nullptr,
                     ImGuiWindowFlags_NoMouseInputs |
                     ImGuiWindowFlags_NoBackground |
                     ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoScrollWithMouse |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoCollapse
                     );
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // No padding

        auto &framebuffer = registry.ctx().get<FrameBuffer>();

        ImVec2 avail = ImGui::GetContentRegionAvail();
        float aspect = (float)constants.WINDOW_WIDTH / (float)constants.WINDOW_HEIGHT;
        float finalWidth = avail.x;
        float finalHeight = finalWidth / aspect;
        if (finalHeight > avail.y) {
            finalHeight = avail.y;
            finalWidth = finalHeight * aspect;
        }

        ImVec2 scenePos = ImGui::GetCursorScreenPos();
        ImVec2 sceneSize = ImGui::GetContentRegionAvail();

        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddImage(
            (ImTextureID)(uintptr_t)framebuffer.colorTexture,
                           scenePos,
                           ImVec2(scenePos.x + sceneSize.x, scenePos.y + sceneSize.y),
                           ImVec2(0, 1), ImVec2(1, 0)  // UVs (flip vertically if needed)
        );
        ImGui::PopStyleVar(); // Pop padding
        ImGui::End();

        // Show Gizmos
        ImGui::SetNextWindowPos(scenePos);
        ImGui::SetNextWindowSize(sceneSize);

        GizmoSystem::Render(registry);
    }

}
