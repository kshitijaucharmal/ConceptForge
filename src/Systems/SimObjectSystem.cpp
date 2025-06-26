#include "SimObjectSystem.hpp"

#include "Components/Rendering/Material.hpp"

#include "glm/gtc/quaternion.hpp"

namespace SimObject {

    // Create entity with default values
    entt::entity Create(entt::registry& registry, std::string name){
        auto entity = registry.create();

        // Add a transform
        registry.emplace<Transform>(entity, Transform{
            .name = name
        });

        return entity;
    }

    glm::mat4 ComposeTransform(const Transform &t) {
        // glm::mat4 mat = glm::mat4(1.0f);
        // mat = glm::translate(mat, t.position);
        // mat = glm::rotate(mat, glm::radians(t.rotation.x), glm::vec3(1, 0, 0));
        // mat = glm::rotate(mat, glm::radians(t.rotation.y), glm::vec3(0, 1, 0));
        // mat = glm::rotate(mat, glm::radians(t.rotation.z), glm::vec3(0, 0, 1));
        // mat = glm::scale(mat, t.scale);

        // Always start with identity matrix
        glm::mat4 model = glm::mat4(1.0f);
        // Apply transformations in the correct order: Scale -> Rotate -> Translate
        model = glm::translate(model, t.position);
        // For rotation, convert Euler angles to quaternion for better interpolation
        glm::quat rotationQuat = glm::quat(glm::radians(t.rotation));
        model = model * glm::mat4_cast(rotationQuat);
        model = glm::scale(model, t.scale);
        return model;
    }

}
