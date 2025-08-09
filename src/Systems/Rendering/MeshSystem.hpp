//
// Created by kshitij on 9/8/25.
//

#pragma once
#include <vector>
#include <Components/Rendering/Mesh.hpp>
#include <Components/Rendering/Shader.hpp>

namespace MeshManager {
    void InitMesh(entt::registry registry, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void Draw(entt::registry &registry, entt::entity mesh, Shader &shader);
}
