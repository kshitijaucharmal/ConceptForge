#include "MaterialSystem.hpp"
#include <Components/Constants.hpp>

namespace MaterialSystem {
    void InitFallbackTexture(entt::registry &registry){
        // Dunno if this is correct
        auto &whiteTexture = registry.ctx().get<FallbackTexture>();
        glGenTextures(1, &whiteTexture);
        glBindTexture(GL_TEXTURE_2D, whiteTexture);
        constexpr unsigned char white[] = { 255, 255, 255, 255 };
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}
