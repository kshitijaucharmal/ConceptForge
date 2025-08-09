//
// Created by kshitij on 30/6/25.
//

#include "PhysicsSystem.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Components/Primitives/Transform.hpp"
#include "Systems/Primitives/PrimitivesSystem.hpp"

#include "Components/Physics/BulletPhysics.hpp"
#include "Components/Physics/Rigidbody.hpp"

#include <btBulletDynamicsCommon.h>

class MotionState : public btMotionState {
public:
    MotionState(const glm::vec3& pos, const glm::quat& rot) : m_pos(pos), m_rot(rot) {}

    void getWorldTransform(btTransform& worldTrans) const override {
        worldTrans.setOrigin(btVector3(m_pos.x, m_pos.y, m_pos.z));
        worldTrans.setRotation(btQuaternion(m_rot.x, m_rot.y, m_rot.z, m_rot.w));
    }

    void setWorldTransform(const btTransform& worldTrans) override {
        const btVector3& p = worldTrans.getOrigin();
        const btQuaternion& r = worldTrans.getRotation();
        m_pos = glm::vec3(p.x(), p.y(), p.z());
        m_rot = glm::quat(r.w(), r.x(), r.y(), r.z());
    }

    glm::vec3 m_pos;
    glm::quat m_rot;
};

namespace BulletPhysicsSystem {
    void Init(entt::registry &registry) {
        auto &bp = registry.ctx().get<BulletPhysics>();
        bp.broadphase = new btDbvtBroadphase();
        bp.collisionConfiguration = new btDefaultCollisionConfiguration();
        bp.dispatcher = new btCollisionDispatcher(bp.collisionConfiguration);
        bp.solver = new btSequentialImpulseConstraintSolver();
        bp.dynamicsWorld = new btDiscreteDynamicsWorld(bp.dispatcher, bp.broadphase, bp.solver, bp.collisionConfiguration);
        bp.dynamicsWorld->setGravity(btVector3(0, -9.81f * 4, 0));
    }

    void Shutdown(entt::registry &registry) {
        auto &bp = registry.ctx().get<BulletPhysics>();
        delete bp.dynamicsWorld;
        delete bp.solver;
        delete bp.dispatcher;
        delete bp.collisionConfiguration;
        delete bp.broadphase;
    }

    void StepSimulation(entt::registry &registry, float deltaTime) {
        auto &bp = registry.ctx().get<BulletPhysics>();
        bp.dynamicsWorld->stepSimulation(deltaTime, 10);
    }

    void SyncTransforms(entt::registry& registry) {
        auto view = registry.view<Rigidbody, Transform>();
        for (auto entity : view) {
            auto& rb = view.get<Rigidbody>(entity);
            auto& transform = view.get<Transform>(entity);

            if (rb.body && rb.body->getMotionState()) {
                btTransform trans;
                rb.body->getMotionState()->getWorldTransform(trans);
                btVector3 pos = trans.getOrigin();
                btQuaternion rot = trans.getRotation();

                transform.position = glm::vec3(pos.x(), pos.y(), pos.z());
                transform.rotation = glm::quat(rot.w(), rot.x(), rot.y(), rot.z());
            }
        }
    }

    void AddRigidbody(entt::entity entity, entt::registry& registry, float mass, Primitives::PrimitiveType objectType) {
        auto &bp = registry.ctx().get<BulletPhysics>();
        auto& transform = registry.get<Transform>(entity);
        auto motionState = new MotionState(transform.position, transform.rotation);

        btCollisionShape *shape = nullptr;
        auto type = registry.get<Primitives::PrimitiveType>(entity);
        const auto scale = btVector3(transform.scale.x, transform.scale.y, transform.scale.z);
        const auto halfScale = btVector3(scale.x()/2, scale.y()/2, scale.z()/2);
        switch (type) {
            case Primitives::PrimitiveType::CUBE: {
                shape = new btBoxShape(btVector3(1, 1, 1));
                shape->setLocalScaling(halfScale); // box half extents
                break;
            }
            case Primitives::PrimitiveType::UV_SPHERE: {
                auto sphere = registry.get<UVSphere>(entity);
                btVector3 positions[] = { btVector3(0, 0, 0) };
                btScalar radii[] = { 1.0f };
                shape = new btMultiSphereShape(positions, radii, 1);
                shape->setLocalScaling(halfScale); // box half extents
                break;
            }
        }

        btVector3 inertia(0, 0, 0);
        if (mass > 0.0f)
            shape->calculateLocalInertia(mass, inertia);

        const btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
        const auto body = new btRigidBody(rbInfo);

        bp.dynamicsWorld->addRigidBody(body);

        Rigidbody rb;
        rb.body = body;
        rb.shape = shape;
        rb.motionState = motionState;
        rb.mass = mass;

        registry.emplace<Rigidbody>(entity, rb);
    }

    void SetBulletTransformFromVisual(const Transform& t, Rigidbody &rb) {
        btTransform bt;
        auto &body = rb.body;

        const btVector3 bulletScale(t.scale.x / 2, t.scale.y / 2, t.scale.z / 2);
        rb.shape->setLocalScaling(bulletScale);

        btVector3 localInertia(0, 0, 0);
        rb.shape->calculateLocalInertia(rb.mass, localInertia);
        body->setMassProps(rb.mass, localInertia);

        bt.setOrigin(btVector3(t.position.x, t.position.y, t.position.z));
        bt.setRotation(btQuaternion(t.rotation.x, t.rotation.y, t.rotation.z, t.rotation.w));

        body->setWorldTransform(bt);
        if (body->getMotionState()) {
            body->getMotionState()->setWorldTransform(bt);
        }

        body->setLinearVelocity(btVector3(0, 0, 0));
        body->setAngularVelocity(btVector3(0, 0, 0));
        body->clearForces();

        body->setWorldTransform(bt);
        body->getMotionState()->setWorldTransform(bt);
        body->activate();
    }
}
