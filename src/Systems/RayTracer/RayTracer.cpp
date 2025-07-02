//
// Created by kshitij on 2/7/25.
//

#include "RayTracer.hpp"
#include "glad/glad.h"

namespace RayTracer {
    void Init(entt::registry &registry) {
        GLuint quadVAO, quadVBO;
        float quadVertices[] = {
            // positions (NDC)
            -1.0f, -1.0f,
             1.0f, -1.0f,
             1.0f,  1.0f,

            -1.0f, -1.0f,
             1.0f,  1.0f,
            -1.0f,  1.0f
        };

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);

        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Render(entt::registry &registry) {

    }


}