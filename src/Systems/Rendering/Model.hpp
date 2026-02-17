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

/**
 * @namespace ModelSystem
 * @brief Provides functionality for loading and initializing 3D models into the registry.
 * conversion of external 3D model formats (via Assimp) into entities
 */
namespace ModelSystem {
    class Model {
    public:
        /**
         * @brief Loads a model file and creates corresponding entities in the registry.
         * @param registry The EnTT registry where entities will be created.
         * @param shader_entity The entity containing the Shader program to be assigned to this model.
         * @param path The filesystem path to the model file.
         * @param transform The initial world-space transform to be applied to the model.
         * @param flipUVs Flag to flip texture coordinates on the y-axis (useful for certain formats/APIs).
         * @param treatAsSingle If true, the loader attempts to merge or treat the meshes as a single logical unit.
         */
        Model(entt::registry &registry,
            const entt::entity& shader_entity,
            const std::string& path,
            Transform& transform,
            bool flipUVs = false,
            bool treatAsSingle = false
            );

    private:
        /// The directory path of the loaded model, used for resolving relative texture paths.
        std::string directory;

        /** @brief Cache of textures already loaded to prevent redundant GPU uploads. */
        std::vector<Texture> textures_loaded;

        /**
         * @brief Recursively traverses the Assimp node tree to process individual meshes.
         * * For every mesh found in a node, a new entity is created and linked with the appropriate components.
         * @param registry The EnTT registry to populate.
         * @param node The current node in the Assimp hierarchy.
         * @param scene The root Assimp scene object.
         * @param shader_entity The shader to associate with the generated mesh entities.
         * @param transform The parent transform to propagate down the hierarchy.
         */
        entt::entity processNode(entt::registry &registry,
            const aiNode *node,
            const aiScene *scene,
            const entt::entity &shader_entity,
            entt::entity parent
            );

        /**
         * @brief Extracts vertex, index, and material data from an Assimp mesh.
         * @param mesh The Assimp mesh data to process.
         * @param scene The root Assimp scene for material lookups.
         * @return A Mesh object ready for GPU buffer initialization.
         */
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        /**
         * @brief Helper utility to load a texture from the disk into the GPU.
         * @param path File name of the texture.
         * @param directory Base directory for the search.
         * @param gamma Whether to perform gamma correction on the texture data.
         * @return The OpenGL texture ID.
         */
        static unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma);

        /**
         * @brief Loads all textures of a specific type (e.g., Diffuse, Specular) for a material.
         * * @param mat The Assimp material to query.
         * @param type The Assimp texture type to look for.
         * @param typeName The internal string name assigned to the texture type (for shader uniforms).
         * @return A vector of Texture objects.
         */
        std::vector<Texture> loadMaterialTextures(const aiMaterial *mat, aiTextureType type, const std::string& typeName);
    };
}
