
#pragma once

#include <entt/entt.hpp>
#include "imgui.h"
#include "ImGuizmo.h"
#include "glm/gtc/type_ptr.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include "glm/gtx/matrix_decompose.hpp"

namespace GizmoSystem {
    void Render(entt::registry &registry);
}
