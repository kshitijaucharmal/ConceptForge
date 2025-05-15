#pragma once

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "imgui.h"
#include "ImGuizmo.h"

#include "constants.hpp"
#include "primitives/entity.hpp"
#include "camera.hpp"
#include "projection.hpp"

namespace Editor{
    class Gizmo {
    public:
        ImGuizmo::OPERATION gizmoOperation = ImGuizmo::TRANSLATE;
        ImGuizmo::MODE gizmoMode = ImGuizmo::LOCAL;
        Gizmo();

        // TODO: Seperate gizmos
        void Show(SimObject::Entity &entity, Projection &projection, Camera &camera);
    };
}
