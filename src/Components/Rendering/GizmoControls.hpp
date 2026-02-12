#pragma once

#include "imgui.h"
#include "ImGuizmo.h"

struct GizmoControls {
    ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
    ImGuizmo::MODE mode = ImGuizmo::WORLD;
};
