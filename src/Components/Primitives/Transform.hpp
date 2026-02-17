#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>

#include "entt/entt.hpp"

struct Transform {
    std::string name = "Object";

    glm::vec3 position = glm::vec3(0.0);
    glm::quat rotation = glm::quat(glm::vec3(0.0f));
    glm::vec3 scale = glm::vec3(1.0);

    glm::mat4 model = glm::mat4(1.0f);

    entt::entity parent = entt::null;
    std::vector<entt::entity> children;

    static void DetachFromParent(entt::registry& registry, entt::entity child) {
        if (!registry.valid(child)) return;

        auto& transform = registry.get<Transform>(child);
        entt::entity oldParent = transform.parent;

        if (oldParent != entt::null && registry.valid(oldParent)) {
            auto& parentTransform = registry.get<Transform>(oldParent);
            auto& children = parentTransform.children;

            // Erase-remove idiom to clean up the vector
            children.erase(std::remove(children.begin(), children.end(), child), children.end());
        }

        transform.parent = entt::null;
    }

    static void AddChild(entt::registry& registry, const entt::entity parent, const entt::entity child) {
        if (!registry.valid(parent) || !registry.valid(child) || parent == child) return;

        // 1. Remove from old parent if it exists
        DetachFromParent(registry, child);

        // Set the parent
        auto& childTransform = registry.get<Transform>(child);
        childTransform.parent = parent;

        // Add to parent's children list
        if (parent != entt::null) {
            auto& parentTransform = registry.get<Transform>(parent);

            auto it = std::find(parentTransform.children.begin(), parentTransform.children.end(), child);

            if (it == parentTransform.children.end()) {
                parentTransform.children.push_back(child);
            }
        }
    }

};
