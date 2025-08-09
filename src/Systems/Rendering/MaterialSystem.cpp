#include "MaterialSystem.hpp"
#include <Components/Constants.hpp>

namespace MaterialSystem {
    void InitFallbackTexture(entt::registry &registry){
        // Dunno if this is correct
        auto &texID = registry.ctx().get<FallbackTexture>();
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);

        // White for now
        constexpr unsigned char fallbackColor[3] = {255, 255, 255};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, fallbackColor);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}
