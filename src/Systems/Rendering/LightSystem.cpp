#include "LightSystem.hpp"

#include <Components/SSBOHolder.hpp>
#include <Components/Rendering/Material.hpp>
#include <Components/Rendering/Shader.hpp>
#include <Core/SSBOManager.hpp>
#include <glad/glad.h>
#include <Systems/Primitives/PrimitivesSystem.hpp>

#include "Components/SceneRoot.hpp"
#include "Components/Rendering/DirectionalLight.hpp"
#include "Components/Rendering/PointLight.hpp"

namespace LightSystem {
    entt::entity AddPointLight(entt::registry &registry, Transform transform, PointLight point_light) {
        // Create a cube with unlit shader
        auto &shaderStore = registry.ctx().get<ShaderStore>().shaders;
        const entt::entity lightEntity = registry.create();
        registry.emplace<Transform>(lightEntity, transform);
        // Requires to be drawn
        registry.emplace<Material>(lightEntity, Material {
            .shader = shaderStore["UnlitShader"],
            .initialized = true
        });
        // TODO: Should be some kind of Gizmo, but an Unshaded UV Sphere for now
        std::vector meshes = {Primitives::CreateUVSphereMesh(registry)};

        registry.emplace<std::vector<Mesh>>(lightEntity, meshes);
        registry.emplace<PointLight>(lightEntity, point_light);

        Transform::Reparent(registry, registry.ctx().get<SceneRoot>().entity, lightEntity);

        return lightEntity;
    }
    entt::entity AddDirectionalLight(entt::registry &registry, const Transform& transform, DirectionalLight directional_light) {
        // Create a cube with unlit shader
        auto &shaderStore = registry.ctx().get<ShaderStore>().shaders;

        const entt::entity lightEntity = registry.create();
        registry.emplace<Transform>(lightEntity, transform);

        // See it visually
        registry.emplace<Material>(lightEntity, Material {
            .shader = shaderStore["UnlitShader"],
            .initialized = true
        });
        std::vector meshes = {Primitives::CreateUVSphereMesh(registry)};

        registry.emplace<std::vector<Mesh>>(lightEntity, meshes);
        registry.emplace<DirectionalLight>(lightEntity, directional_light);

        Transform::Reparent(registry, registry.ctx().get<SceneRoot>().entity, lightEntity);
        return lightEntity;
    }

    void RenderPointLights(entt::registry &registry) {
        const auto entities = registry.view<Transform, PointLight>();
        auto &ssbo = registry.ctx().get<SSBOHolder>().ssbos;

        std::vector<PointLight> pointLights;
        for (const auto &entity : entities) {
            auto [transform, pointLight] = entities.get(entity);
            pointLight.position = transform.position;
            pointLights.push_back(pointLight);
        }

        SSBOManager::UploadVectorToSSBO(ssbo["pointLights"], pointLights);
    }

    void RenderDirectionalLights(entt::registry &registry) {
        const auto entities = registry.view<Transform, DirectionalLight>();
        auto &ssbo = registry.ctx().get<SSBOHolder>().ssbos;

        std::vector<DirectionalLight> dirLights;
        for (const auto &entity : entities) {
            auto [transform, dirLight] = entities.get(entity);
            dirLight.direction = glm::eulerAngles(transform.rotation);
            dirLights.push_back(dirLight);
        }

        SSBOManager::UploadVectorToSSBO(ssbo["dirLights"], dirLights);
    }
}