//
// Created by kshitij on 3/7/25.
//

#ifndef VEC3_HPP
#define VEC3_HPP

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp>
#include <iostream>
#include <random>

using Vec3 = glm::vec3;
using Point3 = glm::vec3;

namespace Vector3 {

    // Random helpers
    inline double RandomDouble(double min = 0.0, double max = 1.0) {
        static thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_real_distribution<double> distribution(min, max);
        return distribution(generator);
    }

    inline Vec3 RandomVec3() {
        return Vec3(RandomDouble(), RandomDouble(), RandomDouble());
    }

    inline Vec3 RandomVec3(double min, double max) {
        return Vec3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
    }

    // Utility functions
    inline Vec3 UnitVector(const Vec3& v) {
        return glm::normalize(v);
    }

    inline double LengthSquared(const Vec3& v) {
        return glm::length2(v);
    }

    inline Vec3 RandomUnitVector() {
        while (true) {
            Vec3 p = RandomVec3(-1.0, 1.0);
            double lenSq = LengthSquared(p);
            if (1e-160 < lenSq && lenSq <= 1.0)
                return glm::normalize(p);
        }
    }

    inline Vec3 RandomOnHemisphere(const Vec3& normal) {
        Vec3 onUnitSphere = RandomUnitVector();
        if (glm::dot(onUnitSphere, normal) > 0.0)
            return onUnitSphere;
        else
            return -onUnitSphere;
    }

    // Utility
    // Binary operators
    inline Vec3 operator*(const Vec3& a, const Vec3& b) {
        return Vec3(a.x * b.x, a.y * b.y, a.z * b.z);
    }

    inline Vec3 operator*(double t, const Vec3& v) {
        return Vec3(t * v.x, t * v.y, t * v.z);
    }

    inline Vec3 operator*(const Vec3& v, double t) {
        return t * v;
    }

    inline Vec3 operator/(const Vec3& v, double t) {
        return (1.0 / t) * v;
    }

    inline Vec3 operator-(const Vec3& v) {
        return Vec3(-v.x, -v.y, -v.z);
    }

    // Dot and cross
    inline double Dot(const Vec3& a, const Vec3& b) {
        return glm::dot(a, b);
    }

    inline Vec3 Cross(const Vec3& a, const Vec3& b) {
        return glm::cross(a, b);
    }

    // Compound assignment operators
    inline Vec3& operator+=(Vec3& a, const Vec3& b) {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
        return a;
    }

    inline Vec3& operator-=(Vec3& a, const Vec3& b) {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;
        return a;
    }

    inline Vec3& operator*=(Vec3& a, double t) {
        a.x *= t;
        a.y *= t;
        a.z *= t;
        return a;
    }

    inline Vec3& operator/=(Vec3& a, double t) {
        return a *= (1.0 / t);
    }

    // Stream output
    inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
        return out << v.x << ' ' << v.y << ' ' << v.z;
    }

} // namespace Vec3

#endif
#endif
