#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>

#include "entt/entt.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include "glm/gtx/matrix_decompose.hpp"

struct Transform {
    std::string name = "Object";

    glm::vec3 position = glm::vec3(0.0);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 eulerAngles = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0);

    glm::mat4 model = glm::mat4(1.0f);

    entt::entity parent        = entt::null;
    entt::entity first_child   = entt::null;
    entt::entity prev_sibling  = entt::null;
    entt::entity next_sibling  = entt::null;

    static glm::vec3 GetWorldPosition(const Transform& t) {
        return glm::vec3(t.model[3]);
    }

    static glm::quat GetWorldRotation(const Transform& t) {
        glm::vec3 s, p; glm::quat r; glm::vec3 sk; glm::vec4 pers;
        glm::decompose(t.model, s, r, p, sk, pers);
        return r;
    }

    static glm::vec3 GetWorldScale(const Transform& t) {
        // A faster way for scale without full decomposition:
        // Get the length of the 3 basis vectors (columns 0, 1, and 2)
        return {
            glm::length(glm::vec3(t.model[0])),
            glm::length(glm::vec3(t.model[1])),
            glm::length(glm::vec3(t.model[2]))
        };
    }

    static void DetachFromParent(entt::registry& registry, const entt::entity entity) {
        auto& node = registry.get<Transform>(entity);

        if (node.parent == entt::null) return;

        auto& parentNode = registry.get<Transform>(node.parent);

        // 1. If this is the "head" child, move the parent's pointer to the next sibling
        if (parentNode.first_child == entity) {
            parentNode.first_child = node.next_sibling;
        }

        // 2. Stitch neighbors together
        if (node.prev_sibling != entt::null) {
            registry.get<Transform>(node.prev_sibling).next_sibling = node.next_sibling;
        }
        if (node.next_sibling != entt::null) {
            registry.get<Transform>(node.next_sibling).prev_sibling = node.prev_sibling;
        }

        // 3. Reset entity pointers
        node.parent = entt::null;
        node.prev_sibling = entt::null;
        node.next_sibling = entt::null;
    }

    static void Reparent(entt::registry& registry, entt::entity parent, entt::entity child) {
        if (parent == child || parent == entt::null) return;

        // Ensure child is detached from any previous parent first
        DetachFromParent(registry, child);

        auto& pNode = registry.get<Transform>(parent);
        auto& cNode = registry.get<Transform>(child);

        // 1. Assign parent
        cNode.parent = parent;
        // 2. Insert at the head of the list
        if (pNode.first_child != entt::null) {
            // Link the current first child to our new child
            auto& oldFirst = registry.get<Transform>(pNode.first_child);
            oldFirst.prev_sibling = child;
            cNode.next_sibling = pNode.first_child;
        }

        // 3. Parent now recognizes this child as the first
        pNode.first_child = child;
    }
};
