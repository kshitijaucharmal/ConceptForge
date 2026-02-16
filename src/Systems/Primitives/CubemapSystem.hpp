//
// Created by kshitij on 2/16/26.
//

#ifndef CONCEPTFORGE_CUBEMAPSYSTEM_HPP
#define CONCEPTFORGE_CUBEMAPSYSTEM_HPP

#include <vector>
#include <string>

/**
 * Cube Map Implementation for rendering skyboxes
 * TODO: Optimization to be made: Render Cube map *AFTER* everything
 */
namespace CubeMap
{
    /**
     * @param folder_path Path of the folder containing six images
     * @brief Uses images from folder to Initialize a Cube map
     */
    void Init(const std::string& folder_path);

    /// Load Cubemap (Used in init)
    void LoadCubeMap(const std::vector<std::string>& faces);
}

#endif //CONCEPTFORGE_CUBEMAPSYSTEM_HPP