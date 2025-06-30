//
// Created by kshitij on 30/6/25.
//

#pragma once

#include <btBulletDynamicsCommon.h>
#include <Components/Physics/Rigidbody.hpp>
#include <Components/Primitives/Transform.hpp>
#include <entt/entt.hpp>

namespace BulletPhysicsSystem {
    void Init(entt::registry& registry);
    void Shutdown(entt::registry &registry);
    void StepSimulation(entt::registry &registry, float deltaTime);
    void SyncTransforms(entt::registry& registry);
    void AddRigidbody(entt::entity entity, entt::registry& registry, float mass, btCollisionShape* shape);
    void SetBulletTransformFromVisual(const Transform& t, Rigidbody &body);
};
