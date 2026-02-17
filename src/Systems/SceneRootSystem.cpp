//
// Created by kshitij on 2/17/26.
//

#include "SceneRootSystem.hpp"

#include "Components/SceneRoot.hpp"

struct SceneRoot;

namespace SceneRootSystem
{
    void InitRoot(entt::registry &registry)
    {
        auto & [transform, entity] = registry.ctx().get<SceneRoot>();
        entity = registry.create();
        registry.emplace<Transform>(entity, transform);
    }
}
