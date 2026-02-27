//
// Created by kshitij on 2/16/26.
//

#include "CubemapSystem.hpp"

#include <format>

#include "stb_image.h"
#include "glad/glad.h"
#include <iostream>

#include "PrimitivesSystem.hpp"
#include "VertexData.hpp"
#include "Components/Primitives/Transform.hpp"
#include "Components/Rendering/Material.hpp"
#include "entt/entt.hpp"
namespace CubeMap {


    void Init(entt::registry &registry, const std::string& folder_path) {
        std::vector<std::string> faces = {
            "right",
            "left",
            "top",
            "bottom",
            "front",
            "back",
        };

        // Add path and extension
        for(auto & face : faces) {
            face = folder_path + face + ".jpg";
        }

        LoadCubeMap(registry, faces);
    }

    void LoadCubeMap(entt::registry &registry, const std::vector<std::string>& faces) {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        unsigned char *data;
        for(unsigned int i = 0; i < faces.size(); i++) {
            data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cube map tex failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        unsigned int VAO, VBO, EBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(glm::vec3), &skyboxVertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(unsigned int), &cubeIndices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

        // Unbind
        glBindVertexArray(0);

        const int indexSize = cubeIndices.size();

        auto mesh =  Mesh{
            .vertices = {},
            .indices = cubeIndices,
            .textures = {},
            .VAO = VAO,
            .VBO = VBO,
            .EBO = EBO,
            .indexCount = indexSize,
            .elemental = true,
            .initialized = true
        };
        auto skyboxShader = registry.ctx().get<ShaderStore>().shaders["SkyboxShader"];

        auto material = Material{
            .shader = skyboxShader,
            .initialized = true
        };

        const entt::entity e = registry.create();
        registry.emplace<Transform>(e, Transform{"Skybox"});
        registry.emplace<Material>(e, material);
        std::vector meshes = {mesh};
        registry.emplace<std::vector<Mesh>>(e, meshes);
        registry.emplace<Skybox>(e);
    }
}
