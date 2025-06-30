#include "LightSystem.hpp"

#include <Components/SSBOHolder.hpp>
#include <Components/Rendering/Shader.hpp>
#include <glad/glad.h>
#include <Systems/Primitives/CubeSystem.hpp>

#include "Components/Rendering/DirectionalLight.hpp"
#include "Components/Rendering/PointLight.hpp"

namespace LightSystem {
    void AddPointLight(entt::registry &registry, Transform transform) {
        // Create a cube with unlit shader
        auto &shaderStore = registry.ctx().get<ShaderStore>().shaders;
        auto lightEntity = CubeSystem::CreateCubeObject(registry, transform, shaderStore["UnlitShader"]);
        registry.emplace<PointLight>(lightEntity, PointLight{
            .position = transform.position,
        });

        auto &handle = registry.ctx().get<PointLightsHandle>();
        handle.entities.push_back(lightEntity);
    }
    void AddDirectionalLight(entt::registry &registry, Transform transform) {
        // Create a cube with unlit shader
        auto &shaderStore = registry.ctx().get<ShaderStore>().shaders;
        auto lightEntity = CubeSystem::CreateCubeObject(registry, transform, shaderStore["UnlitShader"]);
        registry.emplace<DirectionalLight>(lightEntity, DirectionalLight{
            .direction = glm::eulerAngles(transform.rotation)
        });
        auto &handle = registry.ctx().get<DirectionalLightsHandle>();
        handle.entities.push_back(lightEntity);
    }

    void RenderPointLights(entt::registry &registry) {
        const auto &handle = registry.ctx().get<PointLightsHandle>();
        auto &ssbo = registry.ctx().get<SSBOHolder>().ssbos;

        std::vector<PointLight> pointLights;
        for (auto &entity : handle.entities) {
            const auto &transform = registry.get<Transform>(entity);
            auto &pointLight = registry.get<PointLight>(entity);
            pointLight.position = transform.position;
            pointLights.push_back(pointLight);
        }

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo["pointLights"]);
        glBufferData(GL_SHADER_STORAGE_BUFFER, pointLights.size() * sizeof(PointLight), pointLights.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void RenderDirectionalLights(entt::registry &registry) {
        const auto &handle = registry.ctx().get<DirectionalLightsHandle>();
        auto &ssbo = registry.ctx().get<SSBOHolder>().ssbos;

        std::vector<DirectionalLight> dirLights;
        for (auto &entity : handle.entities) {
            auto &transform = registry.get<Transform>(entity);
            auto &dirLight = registry.get<DirectionalLight>(entity);
            dirLight.direction = glm::eulerAngles(transform.rotation);
            dirLights.push_back(dirLight);
        }

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo["dirLights"]);
        glBufferData(GL_SHADER_STORAGE_BUFFER, dirLights.size() * sizeof(DirectionalLight), dirLights.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
}