//
// Created by kshitij on 9/8/25.
//

#pragma once
#include <vector>
#include <Components/Rendering/Mesh.hpp>
#include <Components/Rendering/Shader.hpp>

namespace MeshManager {
    Mesh InitMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Texture> &textures);

    // To Remove
    void Draw(entt::registry &registry, Mesh mesh, Shader &shader);
}
