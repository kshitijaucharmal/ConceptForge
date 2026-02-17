//
// Created by kshitij on 10/8/25.
//

#include "Model.hpp"

#include "Systems/Rendering/MeshSystem.hpp"
#include <stb_image.h>

#include "BulletCollision/Gimpact/gim_linear_math.h"
#include "Components/SceneRoot.hpp"
#include "Components/Primitives/Transform.hpp"
#include "Components/Rendering/Material.hpp"

namespace ModelSystem {
    Model::Model(
        entt::registry& registry,
        const entt::entity& shader_entity,
        const std::string& path,
        Transform &transform,
        const bool flipUVs,
        const bool treatAsSingle
        ) {
        Assimp::Importer import;
        const auto flags = aiProcess_Triangulate
            | aiProcess_GenSmoothNormals
            | aiProcess_CalcTangentSpace
            | (treatAsSingle ? aiProcess_PreTransformVertices : 0)
            | (flipUVs ? aiProcess_FlipUVs : 0);
        const aiScene *scene = import.ReadFile(path, flags);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            printf("ERROR::ASSIMP::%s\n",import.GetErrorString());
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        const auto scene_root = registry.ctx().get<SceneRoot>().entity;
        entity = processNode(registry, scene->mRootNode, scene, shader_entity, entt::null);
        registry.get<Transform>(entity) = transform;
        Transform::Reparent(registry, scene_root, entity);
    }

    entt::entity Model::processNode(
    entt::registry &registry,
    const aiNode *node,
    const aiScene *scene,
    const entt::entity &shader_entity,
    const entt::entity parent
) {
        const entt::entity entity = registry.create();

        // 1. Decompose Assimp Matrix to GLM components
        aiVector3D aiPos, aiRot, aiScale;
        node->mTransformation.Decompose(aiScale, aiRot, aiPos);

        // 2. Convert to GLM types
        glm::vec3 pos = { aiPos.x, aiPos.y, aiPos.z };
        glm::vec3 sca = { aiScale.x, aiScale.y, aiScale.z };

        // Assimp decomposition gives Euler angles in radians
        glm::quat rot = glm::quat(glm::vec3(aiRot.x, aiRot.y, aiRot.z));

        // 3. Emplace Transform with extracted data
        registry.emplace<Transform>(entity, Transform{
            .name = node->mName.C_Str(),
            .position = pos,
            .rotation = rot,
            .eulerAngles = glm::degrees(glm::vec3(aiRot.x, aiRot.y, aiRot.z)),
            .scale = sca
        });

        // 4. Link to parent (handles the linked list pointers)
        if (parent != entt::null) {
            Transform::Reparent(registry, parent, entity);
        }

        // Process meshes
        std::vector<Mesh> meshes;
        for(unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.emplace_back(processMesh(mesh, scene));
        }
        registry.emplace<std::vector<Mesh>>(entity, meshes);
        registry.emplace<Material>(entity, Material{shader_entity, true});

        // Recursively process children
        for(unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(registry, node->mChildren[i], scene, shader_entity, entity);
        }

        return entity;
    }

    Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
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

    std::vector<Texture> Model::loadMaterialTextures(const aiMaterial *mat, const aiTextureType type, const std::string& typeName) {
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

    unsigned int Model::TextureFromFile(const char *path, const std::string &directory, bool gamma)
    {
        auto filename = std::string(path);
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
