#include "SimObjectSystem.hpp"

#include <utility>

#include "Components/SceneRoot.hpp"
#include "Components/Rendering/Material.hpp"

#include "glm/gtc/quaternion.hpp"

namespace SimObject {

    // Create entity with default values
    entt::entity Create(entt::registry& registry, std::string name){
        const auto entity = registry.create();

        // Add a transform
        registry.emplace<Transform>(entity, Transform{
            .name = std::move(name)
        });
        Transform::Reparent(registry, registry.ctx().get<SceneRoot>().entity, entity);

        return entity;
    }

    glm::mat4 ComposeTransform(const Transform &t) {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, t.position);
        model *= glm::mat4_cast(t.rotation);
        model = glm::scale(model, t.scale);
        return model;
    }

    void UpdateHierarchyMatrices(entt::registry& registry, const entt::entity entity, const glm::mat4& parentWorld) {
        auto& transform = registry.get<Transform>(entity);

        // 1. Calculate Local Matrix using your Compose function
        glm::mat4 local = ComposeTransform(transform);

        // 2. Calculate World Matrix (Parent * Local)
        // If it's the root, parentWorld should be identity
        transform.model = parentWorld * local;

        // 3. Propagate to children using the Next-Sibling links
        entt::entity child = transform.first_child;
        while (child != entt::null) {
            UpdateHierarchyMatrices(registry, child, transform.model);
            child = registry.get<Transform>(child).next_sibling;
        }
    }

    void Destroy(entt::registry& registry, const entt::entity entity) {
        if (!registry.valid(entity)) return;

        // 1. SURGERY: Remove from parent/sibling links while the entity is still "alive"
        Transform::DetachFromParent(registry, entity);

        // 2. RECURSION: Destroy children
        const auto& transform = registry.get<Transform>(entity);
        entt::entity child = transform.first_child;
        while (child != entt::null) {
            const entt::entity next = registry.get<Transform>(child).next_sibling;
            Destroy(registry, child); // Recursive call
            child = next;
        }

        // 3. FINAL DEATH: Remove from EnTT
        registry.destroy(entity);
    }

}
