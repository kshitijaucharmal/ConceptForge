#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>

#include "entt/entt.hpp"

struct Transform {
    std::string name = "Object";

    glm::vec3 position = glm::vec3(0.0);
    glm::quat rotation = glm::quat(glm::vec3(0.0f));
    glm::vec3 scale = glm::vec3(1.0);

    glm::mat4 model = glm::mat4(1.0f);

    entt::entity parent = entt::null;
    std::vector<entt::entity> children;
};
