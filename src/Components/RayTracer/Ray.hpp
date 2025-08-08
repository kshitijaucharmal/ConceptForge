//
// Created by kshitij on 3/7/25.
//

#ifndef RAY_HPP
#define RAY_HPP
#include <glm/vec3.hpp>
#include "Vec3.hpp"

struct Ray {
    Point3 Position;
    Vec3 Direction;

    Point3 At(const float t) const {
        return Position + t * Direction;
    }
};

#endif //RAY_HPP
