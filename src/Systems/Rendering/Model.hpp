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

namespace ModelSystem {
    class Model {
    public:
        Model(std::string path);

        void Draw(entt::registry &registry, Shader &shader);

    private:
        // model data
        std::vector<Mesh> meshes;
        std::string directory;

        std::vector<Texture> textures_loaded;

        void loadModel(std::string path);
        void processNode(const aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma);

        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    };
}