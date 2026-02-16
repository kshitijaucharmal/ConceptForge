//
// Created by kshitij on 10/8/25.
//

#pragma once
#include <vector>
#include <Components/Rendering/Mesh.hpp>
#include <Components/Rendering/Shader.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Components/Primitives/Transform.hpp"

namespace ModelSystem {
    // Probably will let this be a class, like a helper
    class InitModel {
    public:
        InitModel(entt::registry &registry,
            const entt::entity& shader_entity,
            const std::string& path,
            const Transform& transform,
            bool flipUVs = false);

    private:
        std::string directory;

        std::vector<Texture> textures_loaded;

        void processNode(entt::registry &registry,
            const aiNode *node,
            const aiScene *scene,
            const entt::entity &shader_entity,
            const Transform &transform
            );
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        static unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma);

        std::vector<Texture> loadMaterialTextures(const aiMaterial *mat, aiTextureType type, const std::string& typeName);
    };
}
