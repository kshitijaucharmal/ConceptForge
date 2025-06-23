#pragma once

#include <glm/glm.hpp>

struct PointLight {
    glm::vec3 position;
    float constant;

    glm::vec3 ambient;
    float linear;

    glm::vec3 diffuse;
    float quadratic;

    glm::vec3 specular;
    float pad = 0.0f; // Optional

    // Constructor for convenience
    PointLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec,
               float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f)
    : position(pos), ambient(amb), diffuse(diff), specular(spec),
    constant(constant), linear(linear), quadratic(quadratic) {}
};

struct DirectionalLight {
    glm::vec3 direction;
    float pad1; // padding for alignment

    glm::vec3 ambient;
    float pad2;

    glm::vec3 diffuse;
    float pad3;

    glm::vec3 specular;
    float pad4;

    // Constructor for convenience
    DirectionalLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
    : direction(dir), ambient(amb), diffuse(diff), specular(spec) {}
};
