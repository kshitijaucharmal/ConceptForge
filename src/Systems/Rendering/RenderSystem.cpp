#include "RenderSystem.hpp"

#include "Components/Primitives/Transform.hpp"
#include "Components/Rendering/Material.hpp"
#include "Components/Rendering/Shader.hpp"

#include "Components/Rendering/Mesh.hpp"
#include "Components/Rendering/MeshFilter.hpp"
#include "Components/Rendering/MeshRenderer.hpp"

#include "Systems/SimObjectSystem.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace RenderSystem {
    void Render(entt::registry& registry){
        auto view = registry.view<Transform, MeshFilter, MeshRenderer, Material>();

        int ctr = 0;
        for (auto entity : view) {
            ctr++;
            auto& t = view.get<Transform>(entity);
            auto& f = view.get<MeshFilter>(entity);
            auto& m = view.get<Material>(entity);

            Shader* shader = registry.try_get<Shader>(m.shader);
            Mesh* mesh = registry.try_get<Mesh>(f.meshEntity);
            if (!shader || !mesh || !shader->initialized || !mesh->initialized) continue;

            // Replace with helpers
            auto model = SimObject::ComposeTransform(t);
            glUseProgram(shader->shaderID);
            glUniformMatrix4fv(glGetUniformLocation(shader->shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(mesh->VAO);
            glDrawArrays(GL_TRIANGLES, 0, mesh->indexCount);
            glBindVertexArray(0);
        }

    }

}
