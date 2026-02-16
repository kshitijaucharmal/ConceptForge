//
// Created by kshitij on 10/8/25.
//

#include "Model.hpp"

#include "Systems/Rendering/MeshSystem.hpp"
#include <stb_image.h>

#include "Components/Primitives/Transform.hpp"
#include "Components/Rendering/Material.hpp"

namespace ModelSystem {
    InitModel::InitModel(
        entt::registry& registry,
        const entt::entity& shader_entity,
        const std::string& path,
        const Transform &transform,
        const bool flipUVs
        ) {
        Assimp::Importer import;
        const auto flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | (flipUVs ? aiProcess_FlipUVs : 0);
        const aiScene *scene = import.ReadFile(path, flags);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            printf("ERROR::ASSIMP::%s\n",import.GetErrorString());
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        // Process recursively
        processNode(registry, scene->mRootNode, scene, shader_entity, transform);
    }

    // TODO: No hierarchy system for now, just stores in meshes.
    // Will contain a parent-child relation later
    void InitModel::processNode(
        entt::registry &registry,
        const aiNode *node,
        const aiScene *scene,
        const entt::entity &shader_entity,
        const Transform &parent
        ) {
        // process all the node's meshes (if any)
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

            // Make it an entity (Transform, Material, Mesh)
            // TODO: Parent-Child Relation to other meshes
            // TODO: Local Transformation apply (Applying base transformation to all now)
            const auto entity = registry.create();
            auto transform = parent;
            transform.name = mesh->mName.C_Str();
            registry.emplace<Transform>(entity, transform);
            registry.emplace<Material>(entity, Material{shader_entity, true});
            registry.emplace<Mesh>(entity, processMesh(mesh, scene));
        }
        // then do the same for each of its children
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            const auto child = node->mChildren[i];
            // TODO: Just passing the main transform as parent, should be actual parent
            processNode(registry, child, scene, shader_entity, parent);
        }
    }

    Mesh InitModel::processMesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex{};
            // process vertex positions, normals and texture coordinates
            glm::vec3 vector;

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;

            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // process indices
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process material
        if(mesh->mMaterialIndex >= 0)
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return MeshManager::InitMesh(vertices, indices, textures);
    }

    std::vector<Texture> InitModel::loadMaterialTextures(const aiMaterial *mat, const aiTextureType type, const std::string& typeName) {
        std::vector<Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for(auto & j : textures_loaded)
            {
                if(std::strcmp(j.path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(j);
                    skip = true;
                    break;
                }
            }
            if(!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), directory, false);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture); // add to loaded textures
            }
        }
        return textures;
    }

    unsigned int InitModel::TextureFromFile(const char *path, const std::string &directory, bool gamma)
    {
        std::string filename = std::string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            printf("Texture failed to load at path: %s\n", path);
            stbi_image_free(data);
        }

        return textureID;
    }
}
