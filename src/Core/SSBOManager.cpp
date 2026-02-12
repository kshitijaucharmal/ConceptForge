#include "SSBOManager.hpp"

#include <iostream>
#include <glad/glad.h>

#include "Components/SSBOHolder.hpp"

namespace SSBOManager {
    void AddAndInit(entt::registry &registry, std::string ssboName, unsigned int ssboIndex) {
        auto & [ssbos, ssboCount] = registry.ctx().get<SSBOHolder>();

        glGenBuffers(1, &ssbos[ssboName]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssboIndex, ssbos[ssboName]);

        // increment SSBO Count
        ssboCount++;
    }

}
