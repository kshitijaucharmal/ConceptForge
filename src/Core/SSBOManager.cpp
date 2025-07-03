#include "SSBOManager.hpp"

#include <iostream>
#include <glad/glad.h>

#include "Components/SSBOHolder.hpp"

namespace SSBOManager {
    void AddAndInit(entt::registry &registry, std::string ssboName, uint ssboIndex) {
        auto &holder = registry.ctx().get<SSBOHolder>();

        glGenBuffers(1, &holder.ssbos[ssboName]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssboIndex, holder.ssbos[ssboName]);

        holder.ssboCount++;
    }

}
