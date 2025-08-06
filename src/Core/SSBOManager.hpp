#pragma once

#include <iostream>
#include <entt/entt.hpp>

#include "glad/glad.h"

namespace SSBOManager {
    void AddAndInit(entt::registry &registry, std::string ssboName, unsigned int ssboIndex);


    template <typename T>
    void UploadVectorToSSBO(GLuint ssbo, const std::vector<T>& data) {
        if (ssbo == 0) {
            std::cerr << "[SSBO ERROR] Tried to upload RayTracerSettings to uninitialized SSBO!" << std::endl;
            return;
        }
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(T), data.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    template <typename T>
    void UploadToSSBO(GLuint ssbo, const T& data) {
        if (ssbo == 0) {
            std::cerr << "[SSBO ERROR] Tried to upload RayTracerSettings to uninitialized SSBO!" << std::endl;
            return;
        }
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T), &data, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
}
