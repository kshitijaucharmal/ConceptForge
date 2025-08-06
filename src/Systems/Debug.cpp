//
// Created by kshitij on 3/7/25.
//

#include "Debug.hpp"

#include <iostream>
#include <Components/Camera.hpp>
#include <Components/Rendering/Shader.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/Primitives/PrimitivesSystem.hpp>
#include <Systems/Rendering/ShaderSystem.hpp>

namespace Debug {
    void Init(entt::registry& registry) {
        glEnable(GL_PROGRAM_POINT_SIZE); // 🔥 Required for gl_PointSize to work
        // Create a VAO + VBO for the point
        GLuint vao, vbo;
        float point[] = { 0.0f, 1.0f, 0.0f };
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        auto &di = registry.ctx().get<DebugInfo>();
        di.VAO = vao;
        di.VBO = vbo;
    }

    void DrawPoint(entt::registry &registry, glm::vec3 point, float size) {

        auto &shaderEntity = registry.ctx().get<ShaderStore>().shaders["DebugShader"];
        auto &shader = registry.get<Shader>(shaderEntity);

        auto &activeCam = registry.ctx().get<ActiveCamera>().camera;
        auto &camera = registry.get<Camera>(activeCam);
        auto &cameraTransform = registry.get<Transform>(activeCam);

        ShaderSystem::Use(shader);
        glm::mat4 model = camera.projection * camera.view;
        ShaderSystem::setMat4(shader, "uMVP", model);

        auto &di = registry.ctx().get<DebugInfo>();

        glBindVertexArray(di.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, di.VBO);
        float pointVec[] = {point.x, point.y, point.z};
        glBufferData(GL_ARRAY_BUFFER, sizeof(pointVec), pointVec, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        ShaderSystem::setFloat(shader, "uPointSize", size); // Set point size in shader
        glDrawArrays(GL_POINTS, 0, 1);
    }
}
