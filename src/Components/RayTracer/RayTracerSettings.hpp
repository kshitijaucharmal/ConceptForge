//
// Created by kshitij on 3/7/25.
//

#pragma once

#include "Vec3.hpp"

struct alignas(16) RayTracerSettings {
    // Group1
    float ImageWidth = 600.0f;
    float ImageHeight = 300.0;
    float padding1[2];           // offset: 8 (pad to 16 bytes)

    Vec3 PixelDeltaU = Vec3(0.0f);
    float pad3;

    Vec3 PixelDeltaV = Vec3(0.0f);
    float pad4;

    Vec3 PixelLoc00 = Vec3(0.0f);
    float pad5;

    Vec3 CameraPosition = Vec3(0.0f);
    float pad6;

    glm::mat4 invProjection;     // offset: 80
    glm::mat4 invView;           // offset: 144
};
// 208 bytes
