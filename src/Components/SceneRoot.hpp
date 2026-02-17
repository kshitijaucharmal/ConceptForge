//
// Created by kshitij on 2/17/26.
//

#ifndef CONCEPTFORGE_SCENEROOT_HPP
#define CONCEPTFORGE_SCENEROOT_HPP
#include "Primitives/Transform.hpp"

/**
 * @brief Each and every node in the scene will be a child of this
 */
struct SceneRoot
{
    /// Transform that has the name root, and only one allowed to have parent entt::null
    Transform transform = Transform{
        .name = "Root",
        .parent = entt::null
    };

    /// Will be initialized by the SceneRootSystem
    entt::entity entity = entt::null;
};

#endif //CONCEPTFORGE_SCENEROOT_HPP