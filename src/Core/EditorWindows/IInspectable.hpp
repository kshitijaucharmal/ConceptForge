//
// Created by kshitij on 29/6/25.
//
#pragma once

#include <Components/Primitives/Transform.hpp>
#include "imgui.h"

// Generic fallback: do nothing
template<typename T>
void DrawComponentUI(T& component) {
    // Not inspectable, fallback — do nothing or log
}

template<>
void DrawComponentUI<Transform>(Transform& t) {
    ImGui::DragFloat3("Position", &t.position.x, 0.1f);
    ImGui::DragFloat3("Rotation", &t.rotation.x, 0.1f);
    ImGui::DragFloat3("Scale",    &t.scale.x,    0.1f);
}