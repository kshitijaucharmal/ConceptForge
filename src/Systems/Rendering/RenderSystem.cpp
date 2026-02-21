#include "RenderSystem.hpp"

#include "Components/Primitives/Transform.hpp"
#include "Components/Rendering/Material.hpp"
#include "Components/Rendering/Shader.hpp"

#include "Components/Rendering/Mesh.hpp"
#include "Components/Rendering/FrameBuffer.hpp"
#include "Components/Constants.hpp"

#include "Systems/Primitives/GizmoSystem.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <BulletInverseDynamics/details/MultiBodyTreeImpl.hpp>
#include <Components/Camera.hpp>
#include <Components/Physics/Rigidbody.hpp>
#include <Core/GameState.hpp>
#include <Core/Physics/PhysicsSystem.hpp>
#include <Systems/Rendering/LightSystem.hpp>

#include "MaterialSystem.hpp"
#include "ShaderSystem.hpp"
#include "Components/Fonts.hpp"
#include "Components/SceneRoot.hpp"
#include "Components/Rendering/LightPassFrameBuffer.hpp"
#include "Core/EditorWindows/Hierarchy.hpp"

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

        // Setup FrameBuffer
        auto &framebuffer = registry.ctx().get<FrameBuffer>();
        framebuffer.colorTexture = colorTexture;
        framebuffer.depthBuffer = depthBuffer;
        framebuffer.frameBufferID = framebufferID;

        // unbind
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void BindFramebuffer(entt::registry &registry) {
        const auto &constants = registry.ctx().get<Constants>();
        const auto &framebuffer = registry.ctx().get<FrameBuffer>();
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.frameBufferID);

        // Clear color
        const auto color = constants.BACKGROUND_COLOR;
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glViewport(0, 0, constants.WINDOW_WIDTH, constants.WINDOW_HEIGHT);
    }

    void UnbindFramebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER,0);
    }

    void Render(entt::registry& registry){
        const auto &constants = registry.ctx().get<Constants>();
        const auto lightPassFB = registry.ctx().get<LightPassFrameBuffer>();

        // Get Selected Entity (TODO: Just a single is selected for now, should be allowed multiple pressing shift)
        const auto selected = registry.ctx().get<Hierarchy::Hierarchy>().selectedEntity;
        // Declare vars for selected
        auto selected_model = glm::mat4(1.0f);
        std::vector<Mesh> selected_meshes;

        // Bind the ShadowMap texture
        glActiveTexture(GL_TEXTURE0 + constants.SHADOW_MAP_UNIT);
        glBindTexture(GL_TEXTURE_2D_ARRAY, lightPassFB.shadowDepthArray);

        // Draw All meshes
        // Everything that has Transform, Mesh and Material will be Rendered
        for (const auto view = registry.view<Transform, std::vector<Mesh>, Material>(); const auto entity : view) {
            const auto& _transform = view.get<Transform>(entity);
            const auto& _material = view.get<Material>(entity);
            auto& _meshes = view.get<std::vector<Mesh>>(entity);

            // Check if anything is missing
            Shader* shader = registry.try_get<Shader>(_material.shader);
            if (!shader || _meshes.empty() || !shader->initialized) continue;

            // Apply the transform
            const auto model = _transform.model;

            if (entity == selected) {
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilMask(0xFF);

                // Store model and meshes
                selected_meshes = _meshes;
                selected_model = model;
            }
            else {
                glStencilMask(0x00);
            }

            // Always use the shader before passing parameters
            ShaderSystem::Use(*shader);
            // Set model matrix
            ShaderSystem::setMat4(*shader, "model", model);
            ShaderSystem::setInt(*shader, "shadowMaps", constants.SHADOW_MAP_UNIT);

            // Set material properties
            ShaderSystem::setVec3(*shader, "material.diffuseColor", _material.diffuseColor);
            ShaderSystem::setVec3(*shader, "material.specularColor", _material.specularColor);
            ShaderSystem::setFloat(*shader, "material.shininess", _material.shininess);

            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            unsigned int unit_counter = 0; // Use one counter for all texture units

            // Texture rendering
            for (auto &_mesh : _meshes)
            {
                for (const auto & texture : _mesh.textures) {
                    glActiveTexture(GL_TEXTURE0 + unit_counter); // Use the main counter

                    std::string number;
                    std::string name = texture.type;

                    if (name == "texture_diffuse") number = std::to_string(diffuseNr++);
                    else if (name == "texture_specular") number = std::to_string(specularNr++);
                    else
                    {
                        printf("[WARN] Texture with name %s cannot be rendered",name.c_str());
                        continue;
                    };

                    ShaderSystem::setInt(*shader, "material." + name + number, unit_counter);
                    glBindTexture(GL_TEXTURE_2D, texture.id);
                    unit_counter++; // Increment for each texture used
                }

                // Drawing
                glBindVertexArray(_mesh.VAO);

                // Draw differently depending on EBO availability
                if (_mesh.elemental) glDrawElements(GL_TRIANGLES, _mesh.indexCount, GL_UNSIGNED_INT, nullptr);
                else glDrawArrays(GL_TRIANGLES, 0, _mesh.indexCount);
            }

            // Unbind
            glBindVertexArray(0);
            glActiveTexture(GL_TEXTURE0);
        }
        // Lights
        glStencilMask(0x00);
        LightSystem::RenderPointLights(registry);
        LightSystem::RenderDirectionalLights(registry);

        if (selected != entt::null && !selected_meshes.empty()) {
            DrawBorder(registry, selected_model, selected_meshes);
        }
    }

    void DrawBorder(entt::registry &registry, const glm::mat4 &model, const std::vector<Mesh> &_meshes)
    {
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        auto borderShader = registry.get<Shader>(registry.ctx().get<ShaderStore>().shaders["BorderShader"]);
        ShaderSystem::Use(borderShader);
        ShaderSystem::setMat4(borderShader, "model", model);

        // 4. Re-draw the meshes with the border shader
        for (auto &_mesh : _meshes)
        {
            glBindVertexArray(_mesh.VAO);
            if (_mesh.elemental)
                glDrawElements(GL_TRIANGLES, _mesh.indexCount, GL_UNSIGNED_INT, nullptr);
            else
                glDrawArrays(GL_TRIANGLES, 0, _mesh.indexCount);
            glBindVertexArray(0);
        }

        glDepthFunc(GL_LESS);
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
    }

    void ShowSceneTexture(entt::registry &registry, GLFWwindow* window, GLuint colorTexture) {
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
            (ImTextureID)static_cast<uintptr_t>(colorTexture),
                           scenePos,
                           ImVec2(scenePos.x + sceneSize.x, scenePos.y + sceneSize.y),
                           ImVec2(0, 1), ImVec2(1, 0)  // UVs (flip vertically if needed)
        );
        ImGui::PopStyleVar(); // Pop padding

        const auto &activeCam = registry.ctx().get<ActiveCamera>().camera;
        auto &camera = registry.get<Camera>(activeCam);
        const auto sceneEnd  = ImVec2(scenePos.x + sceneSize.x, scenePos.y + sceneSize.y);

        auto &io = ImGui::GetIO();
        // Check if the mouse is inside the image rectangle
        if (!io.WantCaptureMouse) {
            if (ImGui::IsMouseHoveringRect(scenePos, sceneEnd) && ImGui::IsMouseDown(ImGuiMouseButton_Right))
            {
                if (!constants.RightMouseCaptured) {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    constants.RightMouseCaptured = true;

                    camera.firstMouse = true;
                }
            }
            else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                if (!constants.LeftMouseCaptured) {
                    constants.LeftMouseCaptured = true;
                }
            }
            else {
                constants.RightMouseCaptured = false;
                constants.LeftMouseCaptured = false;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
        ImGui::PopID();

        ImGui::End();

        ControlBar(registry);

        // Show Gizmos
        ImGui::SetNextWindowPos(scenePos);
        ImGui::SetNextWindowSize(sceneSize);

        GizmoSystem::Render(registry);
    }

    void ControlBar(entt::registry &registry) {
        const auto &io = ImGui::GetIO();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2.0f, 10), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
        ImGui::SetNextWindowBgAlpha(0.4f);

        constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
                                 ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoSavedSettings |
                                 ImGuiWindowFlags_NoFocusOnAppearing |
                                 ImGuiWindowFlags_NoNav;

        ImGui::PushFont(registry.ctx().get<Fonts>().FontsDict["IconFont"]);
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
        ImGui::PopFont();
        ImGui::End();

        ImGui::PopStyleVar(3);
    }

}
