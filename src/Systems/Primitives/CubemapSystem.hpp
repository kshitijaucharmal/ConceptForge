//
// Created by kshitij on 2/16/26.
//

#ifndef CONCEPTFORGE_CUBEMAPSYSTEM_HPP
#define CONCEPTFORGE_CUBEMAPSYSTEM_HPP

#include <vector>
#include <string>

#include "entt/entt.hpp"

/**
 * Cube Map Implementation for rendering skyboxes
 * TODO: Optimization to be made: Render Cube map *AFTER* everything
 */
namespace CubeMap
{
    /**
     * @param registry Entt Registry ref
     * @param folder_path Path of the folder containing six images
     * @brief Uses images from folder to Initialize a Cube map
     */
    void Init(entt::registry &registry, const std::string& folder_path);

    /// Load Cubemap (Used in init)
    void LoadCubeMap(entt::registry &registry, const std::vector<std::string>& faces);

    // Tag
    struct Skybox {};
}

#endif //CONCEPTFORGE_CUBEMAPSYSTEM_HPP