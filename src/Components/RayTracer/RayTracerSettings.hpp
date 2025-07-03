//
// Created by kshitij on 3/7/25.
//

#pragma once

#include "Vec3.hpp"

struct RayTracerSettings {
    // Group1
    float ImageWidth = 600.0f;
    float ImageHeight = 300.0;
    float pad1, pad2;

    Vec3 PixelDeltaU = Vec3(0.0f);
    float pad3;

    Vec3 PixelDeltaV = Vec3(0.0f);
    float pad4;

    Vec3 PixelLoc00 = Vec3(0.0f);
    float pad5;

    Vec3 CameraPosition = Vec3(0.0f);
    float pad6;
};
