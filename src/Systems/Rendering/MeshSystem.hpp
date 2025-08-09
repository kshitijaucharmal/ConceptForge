//
// Created by kshitij on 9/8/25.
//

#pragma once
#include <vector>
#include <Components/Rendering/Mesh.hpp>

namespace MeshManager {
    void InitMesh(entt::registry registry, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
}
