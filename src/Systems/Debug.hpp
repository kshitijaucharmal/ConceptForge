//
// Created by kshitij on 3/7/25.
//

#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace Debug {
    struct DebugInfo {
        unsigned int VAO, VBO;
    };
    void Init(entt::registry &registry);
    void DrawPoint(entt::registry &registry, glm::vec3 point, float size);
}

#endif //DEBUG_HPP
