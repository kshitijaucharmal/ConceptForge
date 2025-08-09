//
// Created by kshitij on 9/8/25.
//

#include "MeshSystem.hpp"

#include "MaterialSystem.hpp"
#include "ShaderSystem.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace MeshManager {
    void InitMesh(entt::registry &registry, const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Texture> &textures) {
        const auto mesh = registry.create();

        unsigned int VAO, VBO, EBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        // Unbind
        glBindVertexArray(0);

        const int indexSize = indices.size();

        registry.emplace<Mesh>(mesh, Mesh{
            .vertices = vertices,
            .indices = indices,
            .textures = textures,
            .VAO = VAO,
            .VBO = VBO,
            .EBO = EBO,
            .indexCount = indexSize,
            .elemental = true,
            .initialized = true
        });
    }

    // TODO: Not ECS, need to refactor
    void Draw(entt::registry &registry, Mesh mesh, Shader &shader){
        const auto &fallback = registry.ctx().get<MaterialSystem::FallbackTexture>();

        const auto textures = mesh.textures;
        const auto VAO = mesh.VAO;
        const auto index = mesh.indexCount;

        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        for(unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = textures[i].type;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++);

            ShaderSystem::setInt(shader, ("material." + name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // Fallback handling:
        // If no diffuse bound at all, bind white texture to first slot
        if (diffuseNr == 1) {
            ShaderSystem::setInt(shader, "material.texture_diffuse1", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, fallback);
        }

        // If no specular bound at all, bind white texture to first specular slot
        if (specularNr == 1) {
            ShaderSystem::setInt(shader, "material.texture_specular1", diffuseNr - 1); // safe slot
            glActiveTexture(GL_TEXTURE0 + (diffuseNr - 1));
            glBindTexture(GL_TEXTURE_2D, fallback);
        }

        glActiveTexture(GL_TEXTURE0);

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
