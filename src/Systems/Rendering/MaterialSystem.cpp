#include "MaterialSystem.hpp"
#include <Components/Constants.hpp>

using whiteTexture = uint;

namespace MaterialSystem {
    void InitMaterials(entt::registry& registry){
        auto view = registry.view<Material>();
        InitWhiteTexture(registry);

        for (auto entity : view) {
            auto& material = view.get<Material>(entity);

            if (!material.initialized && registry.valid(material.shader)) {
                auto& shader = registry.get<Shader>(material.shader);

                if (!shader.initialized) continue;

                std::cout << "Initializing material with shader ID " << shader.shaderID << "\n";
                // ShaderSystem::Use(material.shader);
                // TODO: Implementing
                // unsigned int container2Tex = litShader->LoadTexture(TEXTURE_DIR "/container2.png", "material.diffuseMap", 0, true);
                // unsigned int container2_specularTex = litShader->LoadTexture(TEXTURE_DIR "/container2_specular.png", "material.specularMap", 1, false);
                // ShaderSystem::setFloat(material.shader, "material.shininess", 32.0f);
                material.initialized = true;
            }
        }
    }

    // void ApplyShaderParameters(Material &material) {
    //     ShaderSystem::setVec3(material.shader, "material.diffuseColor", material.diffuseColor);
    //     ShaderSystem::setVec3(material.shader, "material.specularColor", material.specularColor);
    //
    //     ShaderSystem::setInt(material.shader, "material.whiteTexture", material.whiteTexture);
    //     // Need to bind these before this
    //     ShaderSystem::setInt(material.shader, "material.diffuseMap", material.diffuseMap);
    //     ShaderSystem::setInt(material.shader, "material.specularMap", material.specularMap);
    //
    //     ShaderSystem::setFloat(material.shader, "material.shininess", material.shininess);
    // }

    void InitWhiteTexture(entt::registry &registry){
        // Dunno if this is correct
        auto texID = registry.ctx().emplace<whiteTexture>();
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);

        unsigned char whitePixel[3] = {125, 125, 125};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, whitePixel);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

}
