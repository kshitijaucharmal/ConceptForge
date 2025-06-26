#pragma once

#include <entt/entt.hpp>
#include "imgui.h"

namespace RenderSystem {
    void Init(entt::registry &registry);
    void Render(entt::registry& registry);
    void ShowSceneTexture(entt::registry &registry);
}
