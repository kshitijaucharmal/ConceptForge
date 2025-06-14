#pragma once

#include "constants.hpp"

#include "imgui.h"
#include "ImGuizmo.h"

#include "primitives/entity.hpp"

namespace Editor {
    class Inspector {
    public:
        Inspector();

        void Show(SimObject::Entity *entity, ImGuizmo::OPERATION &operation, ImGuizmo::MODE &mode);
    };
}
