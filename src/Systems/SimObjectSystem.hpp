#pragma once

#include <entt/entt.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Components/Primitives/Transform.hpp"

namespace SimObject {
    entt::entity Create(entt::registry &registry, std::string name);
    glm::mat4 ComposeTransform(const Transform &t);
    void UpdateHierarchyMatrices(entt::registry& registry, entt::entity entity, const glm::mat4& parentWorld);
}
