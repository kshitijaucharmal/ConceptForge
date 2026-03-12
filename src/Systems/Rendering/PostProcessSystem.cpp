//
// Created by kshitij on 3/12/26.
//

#include "PostProcessSystem.hpp"

#include "ShaderSystem.hpp"
#include "Components/Constants.hpp"
#include "Components/Rendering/FrameBuffer.hpp"
#include "Components/Rendering/PostProcessContext.hpp"

namespace PostProcessSystem {
    void InitQuad(entt::registry& registry) {
        constexpr float quadVerts[] = {
            // positions (NDC)   // uvs
            -1.f,  1.f, 0.f,    0.f, 1.f,
            -1.f, -1.f, 0.f,    0.f, 0.f,
             1.f, -1.f, 0.f,    1.f, 0.f,
            -1.f,  1.f, 0.f,    0.f, 1.f,
             1.f, -1.f, 0.f,    1.f, 0.f,
             1.f,  1.f, 0.f,    1.f, 1.f,
        };

        GLuint VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1); // uv
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);

        auto& ppCtx = registry.ctx().get<PostProcessContext>();
        ppCtx.quadVAO = VAO;
    }

    void Init(entt::registry& registry) {
        auto& ppCtx    = registry.ctx().get<PostProcessContext>();

        // Render it at lower res
        const auto& constants = registry.ctx().get<Constants>();
        ppCtx.renderWidth  = constants.SCENE_WIDTH / constants.RENDER_FACTOR;
        ppCtx.renderHeight  = constants.SCENE_HEIGHT / constants.RENDER_FACTOR;

        for (int i = 0; i < 2; i++) {
            glGenFramebuffers(1, &ppCtx.pingPongFBOs[i]);
            glBindFramebuffer(GL_FRAMEBUFFER, ppCtx.pingPongFBOs[i]);

            glGenTextures(1, &ppCtx.pingPongTextures[i]);
            glBindTexture(GL_TEXTURE_2D, ppCtx.pingPongTextures[i]);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ppCtx.renderWidth, ppCtx.renderHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ppCtx.pingPongTextures[i], 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                printf("PostProcess Ping-Pong FBO %d incomplete!\n", i);
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        InitQuad(registry);
    }

    void RunPass(entt::registry& registry, Shader& shader, const GLuint inputTex, const GLuint outputFBO) {
        const auto& ppCtx = registry.ctx().get<PostProcessContext>();

        glBindFramebuffer(GL_FRAMEBUFFER, outputFBO);
        glViewport(0, 0, ppCtx.renderWidth, ppCtx.renderHeight);

        // Force clean state for 2D quad
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        ShaderSystem::Use(shader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inputTex);
        ShaderSystem::setInt(shader, "screenTexture", 0);

        // Pass additional uniforms per-effect here (e.g. intensity, resolution)

        glBindVertexArray(ppCtx.quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glEnable(GL_DEPTH_TEST);
    }

    void Apply(entt::registry& registry, const GLuint sceneTexture) {
        auto& ppCtx = registry.ctx().get<PostProcessContext>();
        auto& store = registry.ctx().get<ShaderStore>().shaders;
        const auto& passes = ppCtx.activePasses;

        if (passes.empty()) {
            auto& passthrough = registry.get<Shader>(store["PassthroughShader"]);
            RunPass(registry, passthrough, sceneTexture, ppCtx.pingPongFBOs[0]);
            ppCtx.outputTexture = ppCtx.pingPongTextures[0];
            return;
        }

        GLuint readTex = sceneTexture;
        int writeIdx = 0;

        for (const auto & pass : passes) {
            const GLuint writeFBO = ppCtx.pingPongFBOs[writeIdx];
            auto& shader = registry.get<Shader>(store[pass]);

            RunPass(registry, shader, readTex, writeFBO);

            readTex = ppCtx.pingPongTextures[writeIdx];
            writeIdx = 1 - writeIdx;
        }

        ppCtx.outputTexture = readTex;
    }
}
