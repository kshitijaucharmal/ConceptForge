#include "LightSystem.hpp"

#include <Components/SSBOHolder.hpp>
#include <Components/Rendering/Material.hpp>
#include <Components/Rendering/MeshFilter.hpp>
#include <Components/Rendering/MeshRenderer.hpp>
#include <Components/Rendering/Shader.hpp>
#include <Core/SSBOManager.hpp>
#include <glad/glad.h>
#include <Systems/Primitives/PrimitivesSystem.hpp>

#include "Components/Rendering/DirectionalLight.hpp"
#include "Components/Rendering/PointLight.hpp"

namespace LightSystem {
    void AddPointLight(entt::registry &registry, Transform transform) {
        // Create a cube with unlit shader
        auto &shaderStore = registry.ctx().get<ShaderStore>().shaders;
        const entt::entity lightEntity = registry.create();
        registry.emplace<Transform>(lightEntity, transform);
        registry.emplace<Material>(lightEntity, Material {
            .shader = shaderStore["UnlitShader"],
            .initialized = true
        });
        const entt::entity mesh = Primitives::CreateUVSphereMesh(registry);
        registry.emplace<MeshFilter>(lightEntity, mesh);
        registry.emplace<MeshRenderer>(lightEntity);
        registry.emplace<PointLight>(lightEntity, PointLight{
            .position = transform.position,
        });

        auto &handle = registry.ctx().get<PointLightsHandle>();
        handle.entities.push_back(lightEntity);
    }
    void AddDirectionalLight(entt::registry &registry, Transform transform) {
        // Create a cube with unlit shader
        auto &shaderStore = registry.ctx().get<ShaderStore>().shaders;

        const entt::entity lightEntity = registry.create();
        registry.emplace<Transform>(lightEntity, transform);

        // See it visually
        registry.emplace<Material>(lightEntity, Material {
            .shader = shaderStore["UnlitShader"],
            .initialized = true
        });
        const entt::entity mesh = Primitives::CreateUVSphereMesh(registry);
        registry.emplace<MeshFilter>(lightEntity, mesh);
        registry.emplace<MeshRenderer>(lightEntity);

        registry.emplace<DirectionalLight>(lightEntity, DirectionalLight{
            .direction = glm::eulerAngles(transform.rotation),
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

        SSBOManager::UploadVectorToSSBO(ssbo["pointLights"], pointLights);
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

        SSBOManager::UploadVectorToSSBO(ssbo["dirLights"], dirLights);
    }
}