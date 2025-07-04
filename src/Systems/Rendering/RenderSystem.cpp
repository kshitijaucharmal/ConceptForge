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
#include <BulletInverseDynamics/details/MultiBodyTreeImpl.hpp>
#include <Components/Camera.hpp>
#include <Components/Physics/Rigidbody.hpp>
#include <Core/GameState.hpp>
#include <Core/Physics/PhysicsSystem.hpp>
#include <Systems/Rendering/LightSystem.hpp>

// Font Awesome icon defines (or use full glyphs)
#define ICON_FA_PLAY  "\xef\x81\x8b"  // f04b
#define ICON_FA_PAUSE "\xef\x81\x8c"  // f04c
#define ICON_FA_STEP  "\xef\x81\x91"  // f051 (Step Forward)

namespace RenderSystem {
    void Init(entt::registry &registry){
        const auto &constants = registry.ctx().get<Constants>();

        const int width = constants.WINDOW_WIDTH;
        const int height = constants.WINDOW_HEIGHT;

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
        const auto &constants = registry.ctx().get<Constants>();

        glViewport(0, 0, constants.WINDOW_WIDTH, constants.WINDOW_HEIGHT);

        // Draw All meshes
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
            if (mesh->elemental) {
                glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, nullptr);
            }
            else {
                glDrawArrays(GL_TRIANGLES, 0, mesh->indexCount);
            }
            glBindVertexArray(0);
        }

        // Lights
        LightSystem::RenderPointLights(registry);
        LightSystem::RenderDirectionalLights(registry);
    }

    void ShowSceneTexture(entt::registry &registry, GLFWwindow* window) {
        auto &constants = registry.ctx().get<Constants>();
        // Fixed Screen Position
        ImGui::SetNextWindowPos(ImVec2(constants.SCENE_X, constants.SCENE_Y), ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(ImVec2(constants.SCENE_WIDTH, constants.SCENE_HEIGHT), ImGuiCond_Appearing);
        ImGui::Begin("Scene",
                     nullptr,
                     ImGuiWindowFlags_NoMouseInputs |
                     ImGuiWindowFlags_NoBackground |
                     ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoScrollWithMouse |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoTitleBar
                     );

        auto &framebuffer = registry.ctx().get<FrameBuffer>();

        const auto avail = ImGui::GetContentRegionAvail();
        const auto aspect = static_cast<float>(constants.WINDOW_WIDTH) / static_cast<float>(constants.WINDOW_HEIGHT);
        float finalWidth = avail.x;
        float finalHeight = finalWidth / aspect;
        if (finalHeight > avail.y) {
            finalHeight = avail.y;
            finalWidth = finalHeight * aspect;
        }

        const auto scenePos = ImGui::GetCursorScreenPos();
        const auto sceneSize = avail;

        ImGui::PushID("SceneViewport");

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // No padding
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddImage(
            (ImTextureID)static_cast<uintptr_t>(framebuffer.colorTexture),
                           scenePos,
                           ImVec2(scenePos.x + sceneSize.x, scenePos.y + sceneSize.y),
                           ImVec2(0, 1), ImVec2(1, 0)  // UVs (flip vertically if needed)
        );
        ImGui::PopStyleVar(); // Pop padding

        const auto &activeCam = registry.ctx().get<ActiveCamera>().camera;
        auto &camera = registry.get<Camera>(activeCam);
        ImVec2 sceneEnd  = ImVec2(scenePos.x + sceneSize.x, scenePos.y + sceneSize.y);

        auto &io = ImGui::GetIO();
        // Check if the mouse is inside the image rectangle
        if (!io.WantCaptureMouse) {
            if (ImGui::IsMouseHoveringRect(scenePos, sceneEnd) && ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
                if (!constants.MouseCaptured) {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    constants.MouseCaptured = true;
                    camera.firstMouse = true;
                }
            }
            else {
                constants.MouseCaptured = false;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
        ImGui::PopID();

        ImGui::End();

        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

            ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2.0f, 10), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
            ImGui::SetNextWindowBgAlpha(0.4f);

            const ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
                                     ImGuiWindowFlags_AlwaysAutoResize |
                                     ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoSavedSettings |
                                     ImGuiWindowFlags_NoFocusOnAppearing |
                                     ImGuiWindowFlags_NoNav;

            if (ImGui::Begin("##ControlBar", nullptr, flags)) {
                auto &gameState = registry.ctx().get<GameState>();
                if (ImGui::Button(ICON_FA_PLAY)) {
                    gameState.isPlaying = true;
                    auto view = registry.view<Transform, Rigidbody>();
                    for (auto &entity : view) {
                        auto &transform = view.get<Transform>(entity);
                        auto &rb = view.get<Rigidbody>(entity);
                        BulletPhysicsSystem::SetBulletTransformFromVisual(transform, rb);
                    }
                }

                ImGui::SameLine();
                if (ImGui::Button(ICON_FA_PAUSE)) {
                    gameState.isPlaying = false;
                }
                ImGui::SameLine();
                if (ImGui::Button(ICON_FA_STEP)) {
                    // Step
                }
            }
            ImGui::End();

            ImGui::PopStyleVar(3);
        }

        // Show Gizmos
        ImGui::SetNextWindowPos(scenePos);
        ImGui::SetNextWindowSize(sceneSize);

        GizmoSystem::Render(registry);
    }

}
