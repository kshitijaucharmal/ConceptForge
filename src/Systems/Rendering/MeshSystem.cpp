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
    Mesh InitMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Texture> &textures) {
        unsigned int VAO, VBO, EBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        const GLsizei stride = sizeof(Vertex);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, Normal));

        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, TexCoords));

        // Unbind
        glBindVertexArray(0);

        const int indexSize = indices.size();

        return Mesh{
            .vertices = vertices,
            .indices = indices,
            .textures = textures,
            .VAO = VAO,
            .VBO = VBO,
            .EBO = EBO,
            .indexCount = indexSize,
            .elemental = true,
            .initialized = true
        };
    }

    // TODO: Not ECS, need to refactor
    void Draw(entt::registry &registry, Mesh mesh, Shader &shader) {
        const auto &fallback = registry.ctx().get<MaterialSystem::FallbackTexture>();
        const auto VAO = mesh.VAO;
        const auto index = mesh.indexCount;

        ShaderSystem::Use(shader);

        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int unit_counter = 0; // Use one counter for all texture units

        // --- Main Loop for model's actual textures ---
        for (unsigned int i = 0; i < mesh.textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + unit_counter); // Use the main counter

            std::string number;
            std::string name = mesh.textures[i].type;

            if (name == "texture_diffuse") {
                number = std::to_string(diffuseNr++);
            } else if (name == "texture_specular") {
                number = std::to_string(specularNr++);
            }

            ShaderSystem::setInt(shader, ("material." + name + number).c_str(), unit_counter);
            glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
            unit_counter++; // Increment for each texture used
        }

        // --- Fallback Handling ---
        // If no diffuse map was bound, add the fallback in the next available slot.
        // if (diffuseNr == 1) {
        //     glActiveTexture(GL_TEXTURE0 + unit_counter);
        //     ShaderSystem::setInt(shader, "material.texture_diffuse1", unit_counter);
        //     glBindTexture(GL_TEXTURE_2D, fallback);
        //     unit_counter++;
        // }
        //
        // // If no specular map was bound, add the fallback.
        // if (specularNr == 1) {
        //     glActiveTexture(GL_TEXTURE0 + unit_counter);
        //     ShaderSystem::setInt(shader, "material.texture_specular1", unit_counter);
        //     glBindTexture(GL_TEXTURE_2D, fallback);
        //     unit_counter++;
        // }

        // --- Draw Mesh ---
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, index, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0); // Reset to default
    }
}
