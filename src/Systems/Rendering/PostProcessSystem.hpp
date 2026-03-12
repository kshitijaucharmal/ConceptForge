//
// Created by kshitij on 3/12/26.
//

#ifndef CONCEPTFORGE_POSTPROCESSSYSTEM_HPP
#define CONCEPTFORGE_POSTPROCESSSYSTEM_HPP
#include "Components/Rendering/Shader.hpp"
#include "glad/glad.h"
#include "entt/entt.hpp"

namespace PostProcessSystem
{
    void InitQuad(entt::registry& registry);
    void Init(entt::registry& registry);

    /**
     * Runs a specific pass
     * @param registry entt registry reference
     * @param shader Shader to apply
     * @param inputTex Input Texture to read from
     * @param outputFBO Set the output to this FBO (framebuffer object)
     */
    void RunPass(entt::registry& registry, Shader& shader, GLuint inputTex, GLuint outputFBO);

    /**
     * Applies all the post process shaders
     * @param registry entt registry reference
     * @param sceneTexture Main Scene texture
     */
    void Apply(entt::registry& registry, GLuint sceneTexture);
}

#endif //CONCEPTFORGE_POSTPROCESSSYSTEM_HPP